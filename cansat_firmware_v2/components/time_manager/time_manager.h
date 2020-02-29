#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "FreeRTOS.h"

#define TIME_MANAGER_TASK_STACK_SIZE            (configMINIMAL_STACK_SIZE + 50)
#define TIME_MANAGER_TASK_PRIORITY              tskIDLE_PRIORITY + 3
#define TIME_MANAGER_INTERVAL_MS                1000

void time_manager_init(void);

#endif