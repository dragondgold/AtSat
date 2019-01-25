#ifndef __PRESSURE_MANAGER_H__
#define __PRESSURE_MANAGER_H__

#include "esp_system.h"

// Sensor commands
#define PRESSURE_MANAGER_ADDRESS                0xEC
#define PRESSURE_MANAGER_BASE_CAL_ADDRESS       0xA2
#define PRESSURE_MANAGER_RESET_CMD              0x1E
#define PRESSURE_MANAGER_PRESSURE_OSR_512_CMD   0x42
#define PRESSURE_MANAGER_ADC_RESULT_CMD         0x00
#define PRESSURE_MANAGER_TEMP_OSR_512_CMD       0x52

esp_err_t pressure_manager_init(void);

esp_err_t pressure_manager_do_sample(void);

float pressure_manager_get_pressure(void);
float pressure_manager_get_temperature(void);

#endif // __PRESSURE_MANAGER_H__
