#ifndef __SERVO_MANAGER_H__
#define __SERVO_MANAGER_H__

#include "esp_err.h"
#include "driver/ledc.h"

#define SERVO_MANAGER_PARACHUTE_CHANNEL     LEDC_CHANNEL_0
#define SERVO_MANAGER_BALLOON_CHANNEL       LEDC_CHANNEL_1

#define SERVO_MANAGER_CLOSE_SERVO_DUTY      5.0       // 5 % of 50 Hz
#define SERVO_MANAGER_OPEN_SERVO_DUTY       10.0      // 10 % of 50 Hz

esp_err_t servo_manager_init(void);

void servo_manager_open_parachute(void);
void servo_manager_close_parachute(void);
void servo_manager_open_balloon(void);
void servo_manager_close_balloon(void);

#endif // __SERVO_MANAGER_H__
