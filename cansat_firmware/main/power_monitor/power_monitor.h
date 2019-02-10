#ifndef __POWER_MONITOR_H__
#define __POWER_MONITOR_H__

#include "esp_system.h"
#include <stdbool.h>

#define POWER_MONITOR_PIC16_ADDRESS         0x02
#define POWER_MONITOR_VREF                  (3.30)
#define POWER_MONITOR_ADC_COUNTS            (1024)

// Values in mV and mA
#define POWER_MONITOR_MAX_3V3_CURRENT       500
#define POWER_MONITOR_MAX_3V3_VOLTAGE       3400
#define POWER_MONITOR_MAX_5V_CURRENT        300
#define POWER_MONITOR_MAX_5V_VOLTAGE        5100
#define POWER_MONITOR_MAX_BAT_CURRENT       1200
#define POWER_MONITOR_MAX_BAT_VOLTAGE       4500

typedef struct
{
    float constant;
    uint16_t last_filtered_value;
} filter_data_t;

typedef struct
{
    filter_data_t _filter_current;
    filter_data_t _filter_voltage;
    uint16_t raw_i;         // Raw ADC value
    uint16_t raw_v;         // Raw ADC value
    uint16_t voltage;       // Voltage in mV
    uint16_t current;       // Current in mA
    uint16_t avg_voltage;   // Average voltage in mV
    uint16_t avg_current;   // Average current in mA

    bool overcurrent;
    bool overvoltage;
} power_rail_t;

typedef struct
{
    power_rail_t rail_3v3;
    power_rail_t rail_5v;
    power_rail_t rail_bat;
} power_status_t;

esp_err_t power_monitor_init(void);

void power_monitor_clear_errors(void);

uint16_t power_monitor_get_battery_current(void);
uint16_t power_monitor_get_battery_voltage(void);
uint16_t power_monitor_get_3v3_current(void);
uint16_t power_monitor_get_3v3_voltage(void);
uint16_t power_monitor_get_5v_current(void);
uint16_t power_monitor_get_5v_voltage(void);

bool power_monitor_is_3v3_overcurrent(void);
bool power_monitor_is_5v_overcurrent(void);
bool power_monitor_is_battery_overcurrent(void);
bool power_monitor_is_3v3_overvoltage(void);
bool power_monitor_is_5v_overvoltage(void);
bool power_monitor_is_battery_overvoltage(void);

#endif // __POWER_MONITOR_H__
