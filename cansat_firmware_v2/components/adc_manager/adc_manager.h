#ifndef __ADC_MANAGER_H__
#define __ADC_MANAGER_H__

#include <stdint.h>

#include "app_error.h"
#include "nrf_drv_saadc.h"

#define ADC_MANAGER_BATTERY_CHANNEL                     0
#define ADC_MANAGER_BATTERY_SAMPLES                     16

typedef struct {
    nrf_saadc_value_t samples[ADC_MANAGER_BATTERY_SAMPLES];
    void (*sampling_done_handler)(void);
} adc_manager_cfg_t;

ret_code_t adc_manager_init(void);

ret_code_t adc_manager_start_battery_sample(adc_manager_cfg_t * cfg);

#endif