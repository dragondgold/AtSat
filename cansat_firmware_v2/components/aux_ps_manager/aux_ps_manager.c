#include "aux_ps_manager.h"

#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "nrf_gpio.h"
// #include "power_monitor/power_monitor.h"

#define NRF_LOG_MODULE_NAME aux_ps_manager
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;
static bool enabled = false;

bool aux_ps_manager_init(void) 
{
    NRF_LOG_INFO("Initializing");

    // Create the mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Configure pin
    nrf_gpio_cfg_output(AUX_PS_ENABLE_PIN);
    nrf_gpio_pin_clear(AUX_PS_ENABLE_PIN);

    // Disable auxiliary supplies
    aux_ps_manager_disable();

    NRF_LOG_INFO("Initialized");

    return true;
}

void aux_ps_manager_enable(void)
{
    if(xSemaphoreTake(mutex, AUX_PS_MUTEX_TIMEOUT))
    {
        // power_monitor_clear_errors();
        enabled = true;
        nrf_gpio_pin_set(AUX_PS_ENABLE_PIN);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
        NRF_LOG_WARNING("PS enable timeout");
    }
}

void aux_ps_manager_disable(void) 
{
    if(xSemaphoreTake(mutex, AUX_PS_MUTEX_TIMEOUT))
    {
        enabled = false;
        nrf_gpio_pin_clear(AUX_PS_ENABLE_PIN);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
        NRF_LOG_WARNING("PS disable timeout");
    }
}

bool aux_ps_manager_is_enabled(void)
{
    return enabled;
}