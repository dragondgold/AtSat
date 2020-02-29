#include <stdint.h>
#include <stdbool.h>
#include "rtc_manager.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "task.h"

#include "app_error.h"
#include "nrf_drv_rtc.h"

#define NRF_LOG_MODULE_NAME rtc_manager
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

// Events queue
static StaticQueue_t queue_data;
static QueueHandle_t events_queue;
static uint8_t queue_storage[RTC_MANAGER_MAX_EVENTS * sizeof(rtc_event_t)];

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

// Events thread
static StaticTask_t xTaskBuffer;
static StackType_t xStack[RTC_MANAGER_TASK_STACK_SIZE];
static TaskHandle_t xHandle = NULL;

static rtc_event_t current_event;
static bool rtc_disabled = false;

// RTC peripheral
static const nrf_drv_rtc_t m_rtc = NRF_DRV_RTC_INSTANCE(RTC_MANAGER_RTC_INSTANCE);

__STATIC_INLINE void setup_event(rtc_event_t * event)
{
    if(event->before_start != NULL) event->before_start(event);
    current_event = *event;

    // Set the new RTC value, if we fail execute the elapsed callback now
    nrf_drv_rtc_counter_clear(&m_rtc);
    if(nrf_drv_rtc_cc_set(&m_rtc, 0, event->value, !event->is_ppi) == NRFX_ERROR_TIMEOUT)
    {
        if(event->elapsed != NULL) event->elapsed(event, true);
    }
}

static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    if(int_type == NRFX_RTC_INT_COMPARE0)
    {
        // Let the caller know the event is ready
        if(current_event.elapsed != NULL) current_event.elapsed(&current_event, false);

        // Notify the events thread we finished with this event
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(xHandle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

static void events_thread(void * pvParameters)
{
    UNUSED_PARAMETER(pvParameters);
    uint32_t notification_value;
    rtc_event_t temp_event;

    while(true)
    {
        // Wait for a notification event
        if(xTaskNotifyWait(ULONG_MAX, 0, &notification_value, portMAX_DELAY))
        {
            // Get the next event from the queue
            if(xQueueReceive(events_queue, &temp_event, 50))
            {
                // Re-enable RTC if it's disabled
                if(rtc_disabled)
                {
                    nrf_drv_rtc_enable(&m_rtc);
                }

                // Setup the new event
                setup_event(&temp_event);
            }
            else
            {
                // No more events, disable RTC to save power
                rtc_disabled = true;
                nrf_drv_rtc_disable(&m_rtc);
            }
        }
        else
        {
            NRF_LOG_ERROR("Timeout waiting notification");
        }
    }
}

void rtc_manager_init(void)
{
    nrfx_err_t err;
    rtc_disabled = true;

    // Create events queue
    events_queue = xQueueCreateStatic(RTC_MANAGER_MAX_EVENTS,
                        sizeof(rtc_event_t), queue_storage, &queue_data);
    APP_ERROR_CHECK_BOOL(events_queue != NULL);

    // Create mutex
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);
    APP_ERROR_CHECK_BOOL(mutex != NULL);
    xSemaphoreGive(mutex);

    // Create events thread
    xHandle = xTaskCreateStatic(
                      events_thread,
                      "RTC",
                      RTC_MANAGER_TASK_STACK_SIZE,
                      NULL,
                      RTC_MANAGER_TASK_PRIORITY,
                      xStack,
                      &xTaskBuffer);
    APP_ERROR_CHECK_BOOL(xHandle != NULL);

    // Initialize RTC peripheral
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.reliable = true;
    err = nrf_drv_rtc_init(&m_rtc, &config, rtc_handler);
    APP_ERROR_CHECK_BOOL(err == NRFX_SUCCESS);

    // Disable RTC to save power
    nrf_drv_rtc_disable(&m_rtc);

    NRF_LOG_INFO("RTC manager started");
}

bool rtc_manager_request_event(rtc_event_t * event, TickType_t timeout)
{
    if(xSemaphoreTake(mutex, timeout))
    {
        bool ret = true;
        if(xQueueSend(events_queue, event, timeout))
        {
             xTaskNotify(xHandle, 1, eSetValueWithOverwrite);
        }
        else 
        {
            // Queue is full and we timed out
            ret = false;
        }

        xSemaphoreGive(mutex);
        return ret;
    }
    else {
        // Mutex timeout
        return false;
    }
}

void rtc_manager_event_done(rtc_event_t * event)
{
    if(event->is_ppi && current_event.is_ppi)
    {
        xTaskNotify(xHandle, 1, eSetValueWithOverwrite);
    }
}

void rtc_manager_event_done_from_isr(rtc_event_t * event)
{
    if(event->is_ppi && current_event.is_ppi)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(xHandle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

uint32_t rtc_manager_get_event_address(void)
{
    return nrf_drv_rtc_event_address_get(&m_rtc, NRF_RTC_EVENT_COMPARE_0);
}

uint32_t rtc_manager_get_clear_task_address(void)
{
    return nrf_drv_rtc_task_address_get(&m_rtc, NRF_RTC_TASK_CLEAR);
}