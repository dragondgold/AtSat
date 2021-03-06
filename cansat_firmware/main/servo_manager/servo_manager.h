#ifndef __SERVO_MANAGER_H__
#define __SERVO_MANAGER_H__

#include "esp_err.h"
#include "driver/ledc.h"
#include <stdbool.h>

#define SERVO_MANAGER_PARACHUTE_CHANNEL     LEDC_CHANNEL_0
#define SERVO_MANAGER_BALLOON_CHANNEL       LEDC_CHANNEL_1

#define SERVO_MANAGER_PARACHUTE_CLOSE_SERVO_DUTY        2.0       // 2 % of 50 Hz
#define SERVO_MANAGER_PARACHUTE_OPEN_SERVO_DUTY         4.0       // 5 % of 50 Hz
#define SERVO_MANAGER_BALLOON_CLOSE_SERVO_DUTY          3.5
#define SERVO_MANAGER_BALLOON_OPEN_SERVO_DUTY           2.0

esp_err_t servo_manager_init(void);

bool servo_manager_open_parachute(void);
bool servo_manager_close_parachute(void);
bool servo_manager_open_balloon(void);
bool servo_manager_close_balloon(void);

bool servo_manager_is_parachute_open(void);
bool servo_manager_is_ballon_open(void);

#endif // __SERVO_MANAGER_H__
