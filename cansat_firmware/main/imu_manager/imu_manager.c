#include "imu_manager.h"

#include <math.h>
#include "config/cansat.h"
#include "i2c_manager/i2c_manager.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char* TAG = "imu";

static StaticSemaphore_t sample_mutex_buffer;
static SemaphoreHandle_t sample_mutex;

// Data
static imu_axis_data_t acc_data, gyro_data, mag_data;
static unsigned int acc_range = 4;          // g
static unsigned int gyro_range = 250;       // degree/s

// Constants
// The magnetometer has a range in the XY axis of 1300 uT and
//  2500 uT in the Z axis.
const float mag_factor_xy = 1300.0 / 8192.0;
const float mag_factor_z = 2500.0 / 8192.0;

esp_err_t imu_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    sample_mutex = xSemaphoreCreateMutexStatic(&sample_mutex_buffer);

    esp_err_t err = ESP_OK;

    // Setup accelerometer, 4g range, 125 Hz filter, normal mode
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_RANGE_REG, 0x04);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_BW_REG, 0x0C);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_LPW, 0x00);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring accelerometer");
        return ESP_FAIL;
    }

    // Setup gyroscope, 250 degrees/s, 100 Hz sample rate, normal mode
    err = ESP_OK;
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_RANGE_REG, 0x03);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_BW_REG, 0x07);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_LPM1, 0x00);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring gyroscope");
        return ESP_FAIL;
    }

    // Setup magnetometer, soft reset, 30 Hz sample rate, enable all axis (x,y and z), 4 repetitions x-y axis, 15 repetitions z axis
    err = ESP_OK;
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_REG, 0x83);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_2_REG, 0x38);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_3_REG, 0x84);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_4_REG, 0x04);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_5_REG, 0x0F);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring magnetometer");
        return ESP_FAIL;
    }

    // Give some time for the IMU to take samples
    vTaskDelay(300 / portTICK_PERIOD_MS);

    // Make the first sample
    if(imu_manager_sample_all() != ESP_OK)
    {
        ESP_LOGE(TAG, "Error sampling on init: %d", err);
        return err;
    }

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}

esp_err_t imu_manager_sample_all(void)
{
    ESP_LOGV(TAG, "Starting sample");

    if(xSemaphoreTake(sample_mutex, 100 / portTICK_PERIOD_MS))
    {
        uint8_t acc[6], gyro[6], mag[6];
        esp_err_t err;

        // Read accelerometer bytes (2 bytes per axis)
        ESP_LOGV(TAG, "Starting accelerometer read");
        if((err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, 
            IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_DATA_REG, 6, acc)))
            {
                ESP_LOGE(TAG, "Error reading accelerometer: %d", err);
                xSemaphoreGive(sample_mutex);
                return err;
            }
        // Convert the data to 12-bits
        acc_data.x = ((acc[1] * 256) + (acc[0] & 0xF0)) / 16;
        if(acc_data.x > 2047)
        {
            acc_data.x -= 4096;
        }
        acc_data.y = ((acc[3] * 256) + (acc[2] & 0xF0)) / 16;
        if(acc_data.y > 2047)
        {
            acc_data.y -= 4096;
        }
        acc_data.z = ((acc[5] * 256) + (acc[4] & 0xF0)) / 16;
        if(acc_data.z > 2047)
        {
            acc_data.z -= 4096;
        }

        // Read gyroscope bytes (2 bytes per axis)
        ESP_LOGV(TAG, "Starting gyroscope read");
        if((err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, 
            IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_DATA_REG, 6, gyro)))
            {
                ESP_LOGE(TAG, "Error reading gyroscope: %d", err);
                xSemaphoreGive(sample_mutex);
                return err;
            }
        // Convert the data to 16-bits
        gyro_data.x = (gyro[1] * 256) + gyro[0];
        if(gyro_data.x > 32767)
        {
            gyro_data.x -= 65536;
        }
        gyro_data.y = (gyro[3] * 256) + gyro[2];
        if(gyro_data.y > 32767)
        {
            gyro_data.y -= 65536;
        }
        gyro_data.z = (gyro[5] * 256) + gyro[4];
        if(gyro_data.z > 32767)
        {
            gyro_data.z -= 65536;
        }

        // Read magnetometer bytes (2 bytes per axis)
        ESP_LOGV(TAG, "Starting magnetometer read");
        if((err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, 
            IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_DATA_REG, 6, mag)))
            {
                ESP_LOGE(TAG, "Error reading magnetometer: %d", err);
                xSemaphoreGive(sample_mutex);
                return err;
            }
        // Convert the data to 13-bits
        mag_data.x = ((mag[1] * 256) + (mag[0] & 0xF8)) / 8;
        if (mag_data.x > 4095)
        {
            mag_data.x -= 8192;
        }
        mag_data.y = ((mag[3] * 256) + (mag[2] & 0xF8)) / 8;
        if (mag_data.y > 4095)
        {
            mag_data.y -= 8192;
        }
        mag_data.z = ((mag[5] * 256) + (mag[4] & 0xF8)) / 8;
        if (mag_data.z > 4095)
        {
            mag_data.z -= 8192;
        }

        ESP_LOGV(TAG, "Accelerometer (x,y,z): %d, %d, %d", acc_data.x, acc_data.y, acc_data.z);
        ESP_LOGV(TAG, "Gyroscope (x,y,z): %d, %d, %d", gyro_data.x, gyro_data.y, gyro_data.z);
        ESP_LOGV(TAG, "Magnetometer (x,y,z): %d, %d, %d", mag_data.x, mag_data.y, mag_data.z);

        xSemaphoreGive(sample_mutex);
        return ESP_OK;
    }
    ESP_LOGE(TAG, "Error getting sampling mutex");

    return ESP_FAIL;
}

imu_axis_data_f_t imu_manager_get_acceleration(void)
{
    // Calculate the acceleration in mg
    float factor = (float)(acc_range * 1000) / 4096.0;
    imu_axis_data_f_t data = 
    {
        .x = (float)acc_data.x * factor,
        .y = (float)acc_data.y * factor,
        .z = (float)acc_data.z * factor
    };

    ESP_LOGV(TAG, "Accelerometer mg (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}

imu_axis_data_f_t imu_manager_get_gyro(void)
{
    // Calculate angular velocity in degrees/s
    float factor = (float)gyro_range / 32768.0;
    imu_axis_data_f_t data = 
    {
        .x = (float)gyro_data.x * factor,
        .y = (float)gyro_data.y * factor,
        .z = (float)gyro_data.z * factor
    };

    ESP_LOGV(TAG, "Gyroscope degrees (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}

imu_axis_data_f_t imu_manager_get_magnetometer(void)
{
    // Calculate magnetic field in uT
    imu_axis_data_f_t data = 
    {
        .x = (float)mag_data.x * mag_factor_xy,
        .y = (float)mag_data.y * mag_factor_xy,
        .z = (float)mag_data.z * mag_factor_z
    };

    ESP_LOGV(TAG, "Magnetometer uT (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}
