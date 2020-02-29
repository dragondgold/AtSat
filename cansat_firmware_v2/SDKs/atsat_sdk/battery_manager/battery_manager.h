#ifndef __BATTERY_MANAGER_H__
#define __BATTERY_MANAGER_H__

#include <stdint.h>

typedef struct
{
    // Factor to convert between ADC counts and millivolts
    float conversion_factor;
    float filter_time_constant;
} battery_manager_init_t;

void battery_manager_init(battery_manager_init_t * config);

void battery_manager_add_samples(uint16_t * samples, size_t length);

/** 
 * @brief   Get the last calculated battery voltage.
 * @return  Battery voltage in millivolts.
 */
uint16_t battery_manager_get_voltage(void);

/** 
 * @brief   Get the last calculated SOC for the battery.
 * @return  Battery SOC from 0% to 100%.
 */
uint8_t battery_manager_get_soc(void);

#endif