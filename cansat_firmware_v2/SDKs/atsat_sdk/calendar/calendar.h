#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include <stdint.h>
#include <time.h>

/**
 * @brief   Initialize the calendar instance.
 */
void calendar_init(void);

/**
 * @brief   Set the current date and time.
 * @details Sets the current date and time for the calendar. To prevent drifts over long periods of time this method
 *           should be called from time to time to update the calendar from a reliable source (GPS, Internet, etc).
 *
 * @param[in]   year          Any year in 4 digit format (2019, 2020, etc)
 * @param[in]   monht         Month ranging from 1 to 12
 * @param[in]   day           Day ranging from 1 to 31
 * @param[in]   hour          Hour ranging from 0 to 23
 * @param[in]   minute        Minutes ranging from 0 to 59
 * @param[in]   second        Seconds ranging from 0 to 59
 */
void calendar_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);

/**
 * @brief   Add seconds to the current time.
 * @details Adds the given number of seconds to the current time. This should be called periodically to keep the calendar up-to-date.
 *
 * @param[in]   time_t      Number of seconds to add to the current time.
 */
void calendar_add_seconds(time_t seconds);

/**
 * @brief   Get the current date and time.
 * @details Get the current date and time in the standard tm struct.
 *
 * @return  Pointer to internal tm struct with the current date and time.
 */
struct tm * calendar_get_time(void);

/**
 * @brief   Get the current Unix timestamp.
 *
 * @return  Current Unix timestamp (seconds elapsed since 00:00 hours, Jan 1, 1970 UTC)
 */
time_t calendar_get_timestamp(void);

#endif