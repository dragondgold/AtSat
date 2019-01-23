#include "aux_ps.h"

#include <stdbool.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;
static bool enabled = false;

esp_err_t aux_ps_init(void) 
{
    esp_err_t err;

    // Create the mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Configure pin
    gpio_config_t config;
    config.pin_bit_mask = (1 << AUX_PS_ENABLE_PIN);
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        return err;
    }

    // Disable auxiliary supplies
    aux_ps_disable();

    return err;
}

void aux_ps_enable(void)
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(AUX_PS_ENABLE_PIN, 1);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

void aux_ps_disable(void) 
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = false;
        gpio_set_level(AUX_PS_ENABLE_PIN, 0);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

bool aux_ps_is_enabled(void)
{
    return enabled;
}