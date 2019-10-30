#ifndef __BATTERY_MANAGER_H__
#define __BATTERY_MANAGER_H__

#include "esp_system.h"
#include <stdbool.h>

typedef struct 
{
    uint8_t soc;                    // State of charge in %
    uint16_t volts;                 // Battery voltage in mV
    int16_t avg_current;            // Average current in mA
    uint8_t total_capacity;         // Total battery capacity in mAh
    uint8_t remaining_capacity;     // Remaining battery capacity in mAh
    uint8_t health;                 // Battery health in %
    bool is_charging;               // Is the battery being charged?
    bool charging_at_max;           // Is the battery charging with full current?
} battery_data_t;

#define BATTERY_MANAGER_BATTERY_MAH             600
#define BATTERY_MANAGER_SOC1_SET_THRESHOLD      15      // 15 %
#define BATTERY_MANAGER_SOC1_CLEAR_THRESHOLD    20      // 20 %
#define BATTERY_MANAGER_SOCF_SET_THRESHOLD      5       // 5 %
#define BATTERY_MANAGER_SOCF_CLEAR_THRESHOLD    10      // 10 %
#define BATTERY_MANAGER_OVERRIDE_CURRENT        true

esp_err_t battery_manager_init(void);

battery_data_t battery_manager_get(void);

#endif // __BATTERY_MANAGER_H__
