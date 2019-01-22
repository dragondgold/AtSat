#include "i2c_manager.h"

#include <stdbool.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;
static bool enabled = false;

esp_err_t i2c_manager_init(void)
{
    esp_err_t err;

    // Create the mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Configure pin
    gpio_config_t config;
    config.pin_bit_mask = (1 << I2C_ENABLE_PIN);
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        return err;
    }

    // Disable buffer
    i2c_manager_disable_buffer();

    /******************************************************************/
    /*       SET GENERAL I2C BUS FOR SENSORS AND EXTERNAL MODULES     */
    /******************************************************************/
    // Set I2C bus for sensors and external modules
    i2c_config_t general_i2c_config;
    general_i2c_config.mode = I2C_MODE_MASTER;
    general_i2c_config.sda_io_num = SDA_PIN;
    general_i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
    general_i2c_config.scl_io_num = SCL_PIN;
    general_i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
    general_i2c_config.master.clk_speed = 400000;
    if((err = i2c_param_config(GENERAL_I2C_NUMBER, &general_i2c_config)) != ESP_OK)
    {
        return err;
    }
    // Install I2C driver
    if((err = i2c_driver_install(GENERAL_I2C_NUMBER, general_i2c_config.mode, 0, 0, 0)) != ESP_OK)
    {
        return err;
    }

    /******************************************************************/
    /*                    SET I2C BUS FOR PIC16                       */
    /******************************************************************/
    // Set I2C bus for sensors and external modules
    i2c_config_t pic16_i2c_config;
    pic16_i2c_config.mode = I2C_MODE_MASTER;
    pic16_i2c_config.sda_io_num = SDA_PIC16_PIN;
    pic16_i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
    pic16_i2c_config.scl_io_num = SCL_PIC16_PIN;
    pic16_i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
    pic16_i2c_config.master.clk_speed = 1000000;
    if((err = i2c_param_config(PIC16_I2C_NUMBER, &pic16_i2c_config)) != ESP_OK)
    {
        return err;
    }
    // Install I2C driver
    if((err = i2c_driver_install(PIC16_I2C_NUMBER, pic16_i2c_config.mode, 0, 0, 0)) != ESP_OK)
    {
        return err;
    }

    return err;
}

void i2c_manager_enable_buffer(void)
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = true;
        gpio_set_level(I2C_ENABLE_PIN, 1);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

void i2c_manager_disable_buffer(void) 
{
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        enabled = false;
        gpio_set_level(I2C_ENABLE_PIN, 0);
        xSemaphoreGive(mutex);
    }
    else
    {
        // Timeout!
    }
}

bool i2c_manager_is_buffer_disabled(void)
{
    return enabled;
}