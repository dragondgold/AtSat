#include <stdint.h>

#include "moving_median.h"

void moving_median_init(moving_median_t * filter)
{
    filter->last_output_value = 0;
}

uint32_t moving_median_add_value(moving_median_t * filter, uint32_t value)
{
    // Compute the Moving Median filter (https://stackoverflow.com/questions/5516641/smoothing-function-for-battery-voltage-display-to-reduce-spikes-in-embedded-syst)
    uint32_t output = (float)filter->last_output_value + filter->time_constant * (float)((int32_t)value - (int32_t)filter->last_output_value);
    filter->last_output_value = output;
    return output;
}
