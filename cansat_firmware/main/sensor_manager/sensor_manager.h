#ifndef __SENSOR_MANAGER_H__
#define __SENSOR_MANAGER_H__

#include "esp_system.h"

#define SENSOR_MANAGER_STACK_SIZE       1500
#define SENSOR_MANAGER_TASK_PRIORITY    2
#define SENSOR_MANAGER_AFFINITY         tskNO_AFFINITY

esp_err_t sensor_manager_init(void);

#endif // __SENSOR_MANAGER_H__
