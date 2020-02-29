#ifndef __RTC_MANAGER_H__
#define __RTC_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"

#define RTC_MANAGER_TASK_STACK_SIZE             (configMINIMAL_STACK_SIZE + 50)
#define RTC_MANAGER_TASK_PRIORITY               tskIDLE_PRIORITY + 1
#define RTC_MANAGER_RTC_INSTANCE                2
#define RTC_MANAGER_MAX_EVENTS                  10

typedef struct rtc_event_t rtc_event_t;
struct rtc_event_t
{
    // This is called before the timer starts couting the requested time
    void (*before_start)(rtc_event_t * event);
    // Handler called once the requested time elapsed. This is called from a ISR context!
    void (*elapsed)(rtc_event_t * event, bool error);
    uint32_t value;
    bool is_ppi;
};

void rtc_manager_init(void);

/**
 * @brief Function for requesting a new event to the RTC peripheral.
 * @details The application calls this to request a delay generated using the RTC peripheral which allows
 *           to wait while in low power mode. This is designed to use small delays (usually less than the RTOS,
 *           tick period), for larger delays use the RTOS delay functions.
 *
 * @param[in]   rtc_event_t     Event to add to the queue
 * @param[in]   timeout         Timeout in RTOS ticks to wait in case the queue is full or the manager busy
 *
 * @return      true on success, otherwise false.
 */
bool rtc_manager_request_event(rtc_event_t * event, TickType_t timeout);

/**
 * @brief Finalize a requested PPI event.
 * @details The application MUST call this when finished using a previously requested PPI event so other
 *           events can be processed. This MUST NOT be called from an interrupt context.
 *
 * @param[in]   rtc_event_t     Finished PPI event
 */
void rtc_manager_event_done(rtc_event_t * event);
void rtc_manager_event_done_from_isr(rtc_event_t * event);

uint32_t rtc_manager_get_event_address(void);
uint32_t rtc_manager_get_clear_task_address(void);

#endif