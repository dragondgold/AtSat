#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "system_monitor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "battery_manager.h"
#include "ble_manager.h"
#include "adc_manager.h"

#include "nrf_soc.h"

#define NRF_LOG_MODULE_NAME system_monitor
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

// Monitor thread
static StaticTask_t xTaskBuffer;
static StackType_t xStack[SYSTEM_MONITOR_TASK_STACK_SIZE];
static TaskHandle_t xHandle = NULL;

static adc_manager_cfg_t adc_battery_cfg;

static void sampling_done_handler(void)
{
    // ADC has ended sampling, alert the monitor task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(xHandle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void monitor_thread(void * pvParameters)
{
    UNUSED_PARAMETER(pvParameters);

    ret_code_t err_code;
    uint32_t notification_value;
    TickType_t last_wake_time;

    int32_t temp_value;
    float temperature = 0;

    uint8_t battery_soc = 0;

    while(true)
    {
        last_wake_time = xTaskGetTickCount();
        // Read temperature
        if((err_code = sd_temp_get(&temp_value)) == NRF_SUCCESS)
        {
            temperature = (float)temp_value / 4.0;
            ble_manager_update_temperature(temperature);
        }
        else
        {
            NRF_LOG_ERROR("Error reading temperature: %d", err_code);
        }

        err_code = adc_manager_start_battery_sample(&adc_battery_cfg);
        if(err_code == NRF_SUCCESS) {
            // Sample was started, wait for it to terminate
            if(xTaskNotifyWait(ULONG_MAX, 0, &notification_value, pdMS_TO_TICKS(1000)))
            {
                battery_manager_add_samples((uint16_t *)adc_battery_cfg.samples, ADC_MANAGER_BATTERY_SAMPLES);
                battery_soc = battery_manager_get_soc();
                ble_manager_update_battery(battery_soc);
            }
            else
            {
                // Failed to get ADC data
                NRF_LOG_ERROR("Timeout reading ADC");
            }
        }
        else {
            NRF_LOG_ERROR("Error starting battery sampling: %d,", err_code);
        }

        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(SYSTEM_MONITOR_INTERVAL_MS));
    }
}

void system_monitor_init(void)
{
    // Initialize ADC
    adc_manager_init();
    adc_battery_cfg.sampling_done_handler = sampling_done_handler;

    // Initialize battery manager
    battery_manager_init_t init = 
    {
        .conversion_factor = 0.87912,
        .filter_time_constant = 0.6
    };
    battery_manager_init(&init);

    // Create monitor thread
    xHandle = xTaskCreateStatic(
                      monitor_thread,
                      "Monitor",
                      SYSTEM_MONITOR_TASK_STACK_SIZE,
                      NULL,
                      SYSTEM_MONITOR_TASK_PRIORITY,
                      xStack,
                      &xTaskBuffer);
    APP_ERROR_CHECK_BOOL(xHandle != NULL);

    NRF_LOG_INFO("System monitor started");
    NRF_LOG_FLUSH();
}