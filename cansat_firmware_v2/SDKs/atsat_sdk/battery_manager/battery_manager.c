#include <stdio.h>
#include <stdbool.h>

#include "battery_manager.h"
#include "moving_median/moving_median.h"

// Filter
static moving_median_t filter;

static uint16_t battery_voltage;    // Battery voltage in millivolts
static uint8_t battery_percent;     // Battery percent
static float conversion_factor;     // Factor to convert from ADC counts to millivolts

/** 
 * @brief Function for converting the input voltage (in milli volts) into percentage of 3.0 Volts.
 * @details The calculation is based on a linearized version of the battery's discharge
 *           curve. 3.0V returns 100% battery level. The limit for power failure is 2.1V and
 *           is considered to be the lower boundary.
 *
 *           The discharge curve for CR2032 is non-linear. In this model it is split into
 *           4 linear sections:
 *           - Section 1: 3.0V - 2.9V = 100% - 42% (58% drop on 100 mV)
 *           - Section 2: 2.9V - 2.74V = 42% - 18% (24% drop on 160 mV)
 *           - Section 3: 2.74V - 2.44V = 18% - 6% (12% drop on 300 mV)
 *           - Section 4: 2.44V - 2.1V = 6% - 0% (6% drop on 340 mV)
 *
 *           These numbers are by no means accurate. Temperature and
 *           load in the actual application is not accounted for!
 *
 * @param[in] mvolts The voltage in mV
 * @return    Battery level in percent.
*/
static uint8_t battery_voltage_to_percent(const uint16_t mvolts)
{
    uint8_t battery_level;

    if (mvolts >= 3000)
    {
        battery_level = 100;
    }
    else if (mvolts > 2900)
    {
        battery_level = 100 - ((3000 - mvolts) * 58) / 100;
    }
    else if (mvolts > 2740)
    {
        battery_level = 42 - ((2900 - mvolts) * 24) / 160;
    }
    else if (mvolts > 2440)
    {
        battery_level = 18 - ((2740 - mvolts) * 12) / 300;
    }
    else if (mvolts > 2100)
    {
        battery_level = 6 - ((2440 - mvolts) * 6) / 340;
    }
    else
    {
        battery_level = 0;
    }

    return battery_level;
}

void battery_manager_init(battery_manager_init_t * config)
{
    conversion_factor = config->conversion_factor;
    battery_voltage = 0;

    // Initialize the filter
    filter.time_constant = config->filter_time_constant;
    moving_median_init(&filter);
}

void battery_manager_add_samples(uint16_t * samples, size_t length)
{
    if(length > 0) 
    {
        uint32_t filter_output;

        for(size_t n = 0; n < length; ++n)
        {
            filter_output = moving_median_add_value(&filter, (uint32_t)samples[n]);
        }

        // Calculate the filtered battery voltage
        battery_voltage = (float)filter_output * conversion_factor;
        battery_percent = battery_voltage_to_percent(battery_voltage);
    }
}

uint16_t battery_manager_get_voltage(void)
{
    return battery_voltage;
}

uint8_t battery_manager_get_soc(void)
{
    return battery_percent;
}