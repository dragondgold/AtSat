#include <stdio.h>
#include <stdbool.h>

#include "calendar.h"

static struct tm time_struct = { 0 }, tm_return_time = { 0 };
static time_t time_seconds = 0;

void calendar_init(void)
{
    time_seconds = 0;
}

void calendar_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    time_struct.tm_year = year - 1900;
    time_struct.tm_mon = month - 1;
    time_struct.tm_mday = day;
    time_struct.tm_hour = hour;
    time_struct.tm_min = minute;
    time_struct.tm_sec = second;
    time_seconds = mktime(&time_struct);
}

void calendar_add_seconds(time_t seconds)
{
    time_seconds += seconds;
}

struct tm * calendar_get_time(void)
{
    tm_return_time = *localtime(&time_seconds);
    return &tm_return_time;
}

time_t calendar_get_timestamp(void)
{
    return time_seconds;
}