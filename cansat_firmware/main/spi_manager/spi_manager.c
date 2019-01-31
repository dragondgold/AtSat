#include "spi_manager.h"

#include <stdbool.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_log.h"

static const char* TAG = "spi";

// Mutexes
static StaticSemaphore_t buffer_mutex_buffer;
static SemaphoreHandle_t buffer_mutex;
static bool enabled = false;

static spi_device_handle_t cc1101_spi;

esp_err_t spi_manager_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    // Create the buffer_mutex for this resource
    buffer_mutex = xSemaphoreCreateMutexStatic(&buffer_mutex_buffer);

    // Configure pin for SPI buffer
    gpio_config_t config;
    config.pin_bit_mask = (1 << SPI_ENABLE_PIN);
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring SPI buffer enable: %s", esp_err_to_name(err));
        return err;
    }

    // Configure SPI module
    spi_bus_config_t bus_cfg =
    {
        .miso_io_num = MISO_PIN,
        .mosi_io_num = MOSI_PIN,
        .sclk_io_num = SCLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 256
    };
    if((err = spi_bus_initialize(HSPI_HOST, &bus_cfg, 1)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring SPI bus: %s", esp_err_to_name(err));
        return err;
    }

    // Configure SPI device
    spi_device_interface_config_t dev_cfg =
    {
        .clock_speed_hz = 4*1000*1000,          // Clock at 4 MHz (must be divisor of 80 MHz)
        .mode = 0,                              // SPI mode 0
        .spics_io_num = -1,                     // CS pin not controlled by the SPI module
        .queue_size = 7,                        // Up to 7 transactions may be queued
        .pre_cb = NULL,                         // No pre-transfer callback
    };
    if((err = spi_bus_add_device(HSPI_HOST, &dev_cfg, &cc1101_spi)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error adding SPI device: %s", esp_err_to_name(err));
        return err;
    }

    // Configure CC1101 CS pin
    config.pin_bit_mask = (1 << CS_PIN);
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring CS pin: %s", esp_err_to_name(err));
        return err;
    }
    gpio_set_level(CS_PIN, 1);

    // Disable the SPI buffer on startup
    spi_manager_disable_buffer();

    ESP_LOGI(TAG, "Ready!");

    return err;
}

void spi_manager_enable_buffer(void)
{
    if(xSemaphoreTake(buffer_mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(SPI_ENABLE_PIN, 1);
        xSemaphoreGive(buffer_mutex);
    }
    else
    {
        // Timeout!
    }
}

void spi_manager_disable_buffer(void) 
{
    if(xSemaphoreTake(buffer_mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(SPI_ENABLE_PIN, 0);
        xSemaphoreGive(buffer_mutex);
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