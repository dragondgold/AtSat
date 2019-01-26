#include "i2c_manager.h"

#include <stdbool.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#include "esp_log.h"

static const char* TAG = "i2c";

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;
static bool enabled = false;

static StaticSemaphore_t i2c_mutex_buffers[2];
static SemaphoreHandle_t i2c_mutexes[2];

esp_err_t i2c_manager_init(void)
{
    esp_err_t err;

    // Create the mutexes for the I2C buffer and I2C peripherals
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    i2c_mutexes[0] = xSemaphoreCreateMutexStatic(&i2c_mutex_buffers[0]);
    i2c_mutexes[1] = xSemaphoreCreateMutexStatic(&i2c_mutex_buffers[1]);

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

esp_err_t i2c_manager_acquire(i2c_port_t port, TickType_t timeout)
{
    if(port > 1 || port < 0)
    {
        return ESP_FAIL;
    }

    if(xSemaphoreTake(i2c_mutexes[port], timeout)) return ESP_OK;
    else return ESP_FAIL;
}

esp_err_t i2c_manager_release(i2c_port_t port)
{
    if(port > 1 || port < 0)
    {
        return ESP_FAIL;
    }

    if(xSemaphoreGive(i2c_mutexes[port])) return ESP_OK;
    else return ESP_FAIL;
}

esp_err_t i2c_manager_read_register(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, uint8_t* value)
{
    esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(port, timeout)) != ESP_OK)
    {
        return err;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr | 0x01, true);
    i2c_master_read_byte(cmd, value, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    // Send everything, this will block until everything is sent
    err = i2c_master_cmd_begin(port, cmd, timeout);
    i2c_cmd_link_delete(cmd);

    // Release the bus
    i2c_manager_release(port);
    return err;
}

esp_err_t i2c_manager_read_register_multiple(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, unsigned int length, uint8_t* values)
{
    esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(port, timeout)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error acquiring port %d on read", port);
        return err;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr | 0x01, true);
    i2c_master_read(cmd, values, length, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    // Send everything, this will block until everything is sent
    err = i2c_master_cmd_begin(port, cmd, timeout);
    i2c_cmd_link_delete(cmd);

    // Release the bus
    i2c_manager_release(port);
    return err;
}

esp_err_t i2c_manager_write_register_multiple(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, unsigned int length, uint8_t* values)
{
    esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(port, timeout)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error acquiring port %d on write", port);
        return err;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, slave_addr, true);
    i2c_master_write(cmd, values, length, true);
    i2c_master_stop(cmd);

    // Send everything, this will block until everything is sent
    err = i2c_master_cmd_begin(port, cmd, timeout);
    i2c_cmd_link_delete(cmd);

    // Release the bus
    i2c_manager_release(port);
    return err;
}

esp_err_t i2c_manager_write_register(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, uint8_t value)
{
    return i2c_manager_write_register_multiple(port, timeout, slave_addr, reg_addr, 1, &value);
}