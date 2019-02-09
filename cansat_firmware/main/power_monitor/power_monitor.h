#ifndef __POWER_MONITOR_H__
#define __POWER_MONITOR_H__

#include "esp_system.h"
#include <stdbool.h>

typedef struct
{
    uint16_t raw_3v3_v;
    uint16_t raw_5v_v;
    uint16_t raw_3v3_i;
    uint16_t raw_5v_i;
    uint16_t raw_bat_i;
} power_status_t;


#define POWER_MONITOR_PIC16_ADDRESS         0x02

esp_err_t power_monitor_init(void);

#endif // __POWER_MONITOR_H__
