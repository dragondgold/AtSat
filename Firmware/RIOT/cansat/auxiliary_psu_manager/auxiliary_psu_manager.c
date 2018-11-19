#include "auxiliary_psu_manager.h"
#include "periph/gpio.h"
#include "cansat.h"
#include "mutex.h"
#include <stdbool.h>

static mutex_t lock;
static bool is_psu_on = false;

void auxiliary_psu_manager_init(void) 
{
    mutex_init(&lock);

    is_psu_on = false;
    auxiliary_psu_manager_turn_off();
}

void auxiliary_psu_manager_turn_on(void) 
{
    is_psu_on = true;
    gpio_set(GPIO_PIN(ENABLE_5V_PORT, ENABLE_5V_PIN));
    gpio_set(GPIO_PIN(ENABLE_3V3_PORT, ENABLE_3V3_PIN));
}

void auxiliary_psu_manager_turn_off(void)
{
    is_psu_on = false;
    gpio_clear(GPIO_PIN(ENABLE_5V_PORT, ENABLE_5V_PIN));
    gpio_clear(GPIO_PIN(ENABLE_3V3_PORT, ENABLE_3V3_PIN));
}

bool auxiliary_psu_manager_is_on(void)
{
    return is_psu_on;
}

void auxiliary_psu_manager_acquire(void)
{
    mutex_lock(&lock);
}

void auxiliary_psu_manager_release(void)
{
    mutex_unlock(&lock);
}