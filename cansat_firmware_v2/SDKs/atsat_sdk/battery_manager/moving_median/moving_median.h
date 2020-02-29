#ifndef __MOVING_MEDIAN_H__
#define __MOVING_MEDIAN_H__

#include <stdint.h>

typedef struct
{
    uint32_t last_output_value;
    float time_constant;    // time_constant = dt/tau
} moving_median_t;

void moving_median_init(moving_median_t * filter);
uint32_t moving_median_add_value(moving_median_t * filter, uint32_t value);

#endif