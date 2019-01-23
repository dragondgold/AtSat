#include "spi_manager.h"

#include <stdbool.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;
static bool enabled = false;

esp_err_t spi_manager_init(void)
{
    esp_err_t err;

    // Create the mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Configure pin
    gpio_config_t config;
    config.pin_bit_mask = (1 << SPI_ENABLE_PIN);
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        return err;
    }

    spi_manager_disable_buffer();

    return err;
}

void spi_manager_enable_buffer(void)
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(SPI_ENABLE_PIN, 1);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

void spi_manager_disable_buffer(void) 
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(SPI_ENABLE_PIN, 0);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

bool spi_manager_is_buffer_disabled(void)
{
    return enabled;
}