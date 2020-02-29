#ifndef __SYSTEM_MONITOR_H__
#define __SYSTEM_MONITOR_H__

#include "FreeRTOS.h"

#define SYSTEM_MONITOR_TASK_STACK_SIZE          (configMINIMAL_STACK_SIZE + 50)
#define SYSTEM_MONITOR_TASK_PRIORITY            tskIDLE_PRIORITY + 1
#define SYSTEM_MONITOR_INTERVAL_MS              10000  // 15 minutes

void system_monitor_init(void);

#endif