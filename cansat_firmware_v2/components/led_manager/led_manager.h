#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include <stdbool.h>
#include "nrf_gpio.h"
#include "FreeRTOS.h"

#define LED_MANAGER_MUTEX_TIMEOUT               (100 / portTICK_PERIOD_MS)

#define LED_MANAGER_LED_PIN                     NRF_GPIO_PIN_MAP(0, 13)
#define LED_MANAGER_COUNTER_TOP_VALUE           10000
#define LED_MANAGER_PWM_STEPS                   25
#define LED_MANAGER_SLOW_BLINK_REPEATS          5
#define LED_MANAGER_FAST_BLINK_REPEATS          1

bool led_manager_init(void);

void led_manager_off(void);
void led_manager_on(void);
void led_manager_slow_blink(bool faded);
void led_manager_fast_blink(bool faded);

#endif // __LED_MANAGER_H__
