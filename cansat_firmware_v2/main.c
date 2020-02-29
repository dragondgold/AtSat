#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_freertos.h"
#include "nrf_sdm.h"
#include "boards.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_lbs.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_clock.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_ext_flash.h"
#include "ble_manager.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "app_error_weak.h"
#include "ext_flash_manager.h"
#include "flash_hal.h"
#include "assert.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "system_monitor.h"
#include "time_manager.h"
#include "rtc_manager.h"

// Value used as error code on stack dump, can be used to identify stack location on stack unwind
#define DEAD_BEEF                       0xDEADBEEF

static const ext_flash_manager_cfg_t flash_cfg = {
    .flash_driver = {
        .init = flash_hal_init,
        .set_cs = flash_hal_set_cs,
        .set_wp = flash_hal_set_wp,
        .exchange_byte = flash_hal_exchange_byte,
        .write_bytes = flash_hal_write_bytes,
        .read_bytes = flash_hal_read_bytes,
        .max_pages = 32752
    },
    .max_log_size = 100000,
    .max_data_size = 6900000
};
static TaskHandle_t m_logger_thread;

/*
 * Clear FPU exceptions.
 * Without this step, the FPU interrupt is marked as pending,
 * preventing system from sleeping. Exceptions cleared:
 * - IOC - Invalid Operation cumulative exception bit.
 * - DZC - Division by Zero cumulative exception bit.
 * - OFC - Overflow cumulative exception bit.
 * - UFC - Underflow cumulative exception bit.
 * - IXC - Inexact cumulative exception bit.
 * - IDC - Input Denormal cumulative exception bit.
 */
void FPU_IRQHandler(void)
{
    // Prepare pointer to stack address with pushed FPSCR register (0x40 is FPSCR register offset in stacked data)
    uint32_t * fpscr = (uint32_t * )(FPU->FPCAR + 0x40);

    // Execute FPU instruction to activate lazy stacking
    (void)__get_FPSCR();

    // Clear flags in stacked FPSCR register. To clear IDC, IXC, UFC, OFC, DZC and IOC flags, use 0x0000009F mask.
    *fpscr = *fpscr & ~(0x0000009F);
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
#ifdef DEBUG
    //NRF_BREAKPOINT_COND;
#endif
    __disable_irq();
    NRF_LOG_FINAL_FLUSH();

#ifndef DEBUG
    NRF_LOG_ERROR("Fatal error");
#else
    switch (id)
    {
#if defined(SOFTDEVICE_PRESENT) && SOFTDEVICE_PRESENT
        case NRF_FAULT_ID_SD_ASSERT:
            NRF_LOG_ERROR("SOFTDEVICE: ASSERTION FAILED");
            break;
        case NRF_FAULT_ID_APP_MEMACC:
            NRF_LOG_ERROR("SOFTDEVICE: INVALID MEMORY ACCESS");
            break;
#endif
        case NRF_FAULT_ID_SDK_ASSERT:
        {
            assert_info_t * p_info = (assert_info_t *)info;
            NRF_LOG_ERROR("ASSERTION FAILED at %s:%u",
                          p_info->p_file_name,
                          p_info->line_num);
            break;
        }
        case NRF_FAULT_ID_SDK_ERROR:
        {
            error_info_t * p_info = (error_info_t *)info;
            NRF_LOG_ERROR("ERROR %u [%s] at %s:%u\r\nPC at: 0x%08x",
                          p_info->err_code,
                          nrf_strerror_get(p_info->err_code),
                          p_info->p_file_name,
                          p_info->line_num,
                          pc);
             NRF_LOG_ERROR("End of error report");
            break;
        }
        default:
            NRF_LOG_ERROR("UNKNOWN FAULT at 0x%08X", pc);
            break;
    }
#endif

    NRF_BREAKPOINT_COND;
    // On assert, the system can only recover with a reset.

#ifndef DEBUG
    NRF_LOG_WARNING("System reset");
    NVIC_SystemReset();
#else
    app_error_save_and_stop(id, pc, info);
#endif // DEBUG
}

void __assert(const char *__expression, const char *__filename, int __line) {
    NRF_BREAKPOINT_COND;
}

/**
 * Handler for hard faults
 */
void HardFault_Handler(void)
{
    uint32_t* sp = (uint32_t*)__get_MSP(); // Get stack pointer
    uint32_t ia = sp[12]; // Get instruction address from stack

    NRF_LOG_ERROR("Hard Fault at address: 0x%08x\r\n", (unsigned int)ia);
    NRF_LOG_FINAL_FLUSH();
}

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                          StackType_t **ppxIdleTaskStackBuffer,
                                          uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    // Pass out a pointer to the StaticTask_t structure in which the idle tasks state will be stored.
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    // Pass out the array that will be used as the Idle tasks stack.
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    // Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    //  configMINIMAL_STACK_SIZE is specified in words, not bytes.
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize )
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    // Pass out a pointer to the StaticTask_t structure in which the Timer tasks state will be stored.
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    // Pass out the array that will be used as the Timer tasks stack.
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    // Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    //  configTIMER_TASK_STACK_DEPTH is specified in words, not bytes.
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * @brief Initializes LEDs.
 * @details Initializes LEDs on the development board
 */
static void leds_init(void)
{
    bsp_board_init(BSP_INIT_LEDS);
}

/**
 * @brief Initialize the UART and Flash memory loggers.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    // Init UART backend
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Init external flash backend
    NRF_LOG_BACKEND_EXT_FLASH_DEF(ext_flash_log_backend);
    nrf_log_backend_ext_flash_init();
    int32_t backend_id = nrf_log_backend_add(&ext_flash_log_backend, NRF_LOG_SEVERITY_DEBUG);
    ASSERT(backend_id >= 0);
    nrf_log_backend_enable(&ext_flash_log_backend);
}


/**
 * @brief Initializes power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for initializing the clock.
 */
static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for starting advertising. 
 */
static void advertising_start_thread(void * p_erase_bonds)
{
    ble_manager_advertising_start();
}

/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);

    while (1)
    {
        NRF_LOG_FLUSH();
        // Suspend myself
        vTaskSuspend(NULL);
    }
}

/**
 * @brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
void vApplicationIdleHook( void )
{
    vTaskResume(m_logger_thread);
}


/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize logger
    log_init();
    clock_init();

    NRF_LOG_INFO("Booting...");

    // Do not start any interrupt that uses system functions before system initialisation.
    // The best solution is to start the OS before any other initalisation.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }

    // Activate deep sleep mode.
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Initialize flash manager
    NRF_LOG_INFO("Initializing flash manager");
    ext_flash_manager_init(&flash_cfg);

    // Initialize power management.
    power_management_init();

    leds_init();
    ble_manager_init();
    time_manager_init();
    rtc_manager_init();

    // Create a FreeRTOS task for the BLE stack.
    // The task will run advertising_start() before entering its loop.
    nrf_sdh_freertos_init(advertising_start_thread, NULL);
    system_monitor_init();

    // Enable FPU exceptions interrupt
    NVIC_SetPriority(FPU_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_ClearPendingIRQ(FPU_IRQn);
    NVIC_EnableIRQ(FPU_IRQn);

    // Start execution.
    NRF_LOG_INFO("System init complete. Starting scheduler.");
    NRF_LOG_FLUSH();

    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    for (;;)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}

/**
 * @}
 */
