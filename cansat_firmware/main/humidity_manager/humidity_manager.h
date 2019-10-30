#ifndef __TEMP_HUM_MANAGER_H__
#define __TEMP_HUM_MANAGER_H__

#include "esp_system.h"

#define HUM_MANAGER_DEVICE_ADDRESS                 0x80
#define HUM_MANAGER_MEASURE_HUM_NO_HOLD_CMD        0xF5

esp_err_t humidity_manager_init(void);

esp_err_t humidity_manager_sample(void);
unsigned int humidity_manager_get_humidity(void);

#endif // __TEMP_HUM_MANAGER_H__
