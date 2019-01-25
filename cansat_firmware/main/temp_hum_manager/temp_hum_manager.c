#include "temp_hum_manager.h"

#include "config/cansat.h"
#include "i2c_manager/i2c_manager.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char* TAG = "humidity";

static unsigned int humidity;

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

static esp_err_t send_cmd(uint8_t command)
{
    esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(GENERAL_I2C_NUMBER, 500 / portTICK_PERIOD_MS)) != ESP_OK)
    {
        return err;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TEMP_HUM_MANAGER_DEVICE_ADDRESS, true);
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);

    // Send everything, this will block until everything is sent
    err = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // Release the bus
    i2c_manager_release(GENERAL_I2C_NUMBER);
    return err;
}

/**
 * @brief Manager for the temperature and humidty sensor Si7006-A20
 * @return esp_err_t 
 */
esp_err_t temp_hum_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    humidity = 0;

    // Make the first sample
    temp_hum_manager_sample();

    ESP_LOGI(TAG, "Initialized!");

    return ESP_OK;
}

esp_err_t temp_hum_manager_sample(void)
{
    ESP_LOGV(TAG, "Starting sample");
    if(xSemaphoreTake(mutex, 100 / portTICK_PERIOD_MS))
    {
        esp_err_t err;

        // Start humidity sample
        if((err = send_cmd(TEMP_HUM_MANAGER_MEASURE_HUM_NO_HOLD_CMD)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error starting sample: %d", err);
            xSemaphoreGive(mutex);
            return err;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);

        // Read the 2 bytes from the humidity
        ESP_LOGV(TAG, "Reading humidity");
        if((err = i2c_manager_acquire(GENERAL_I2C_NUMBER, 500 / portTICK_PERIOD_MS)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error reading sample: %d", err);
            xSemaphoreGive(mutex);
            return err;
        }

        // Create the commands
        uint8_t buffer[2];
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, TEMP_HUM_MANAGER_DEVICE_ADDRESS | 0x01, true);
        i2c_master_read(cmd, buffer, 2, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        err = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, 100 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
        i2c_manager_release(GENERAL_I2C_NUMBER);

        if(err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error on I2C: %d", err);
            xSemaphoreGive(mutex);
            return err;
        }

        // The LSB and MSB are ready, calculate the humidity. MSB comes first.
        ESP_LOGV(TAG, "Converting humidity");
        uint16_t v = (((uint16_t)buffer[0]) << 8) | buffer[1];
        int16_t h = (int16_t)(((125*(uint32_t)v) / 65536) - 6);

        // The datasheet says the humidity can go over 100% and the app code should
        //  clamp it, so, here it is
        if(h > 100) 
        {
            humidity = 100;
        }
        else if(h < 0) 
        {
            humidity = 0;
        }
        else 
        {
            humidity = h;
        }

        xSemaphoreGive(mutex);
        ESP_LOGV(TAG, "Humidity: %d", humidity);

        return ESP_OK;
    }
    ESP_LOGE(TAG, "Error getting sampling mutex");
    
    return ESP_FAIL;
}