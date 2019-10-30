#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "esp_err.h"
#include "driver/ledc.h"

#define LED_MANAGER_LED_CHANNEL         LEDC_CHANNEL_2
#define LED_MANAGER_SLOW_BLINK_TIME     1000            // In ms
#define LED_MANAGER_FAST_BLINK_TIME     300             // In ms

esp_err_t led_manager_init(void);

void led_manager_off(void);
void led_manager_on(void);
void led_manager_slow_blink(bool faded);
void led_manager_fast_blink(bool faded);
bool led_manager_is_on(void);

#endif // __LED_MANAGER_H__
