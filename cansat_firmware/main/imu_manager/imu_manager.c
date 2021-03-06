#include "imu_manager.h"

#include <math.h>
#include "config/cansat.h"
#include "i2c_manager/i2c_manager.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

//#define LOG_LOCAL_LEVEL     ESP_LOG_DEBUG
static const char* TAG = "imu";

static StaticSemaphore_t sample_mutex_buffer;
static SemaphoreHandle_t sample_mutex;

// Data
static imu_axis_data_t acc_data, gyro_data, mag_data;
static float counts_per_g = 512;                        // 512 counts per g as the accelerometer is in the +-4g range (12-bit)
static float gyro_countsper_deg_per_sec = 16.384;       // 16.384 counts per degree/s as the gyro is in the +-2000 degree/s range (16-bit)

// Constants
// The magnetometer has a range in the XY axis of 1300 uT and
//  2500 uT in the Z axis.
const float mag_counts_per_ut_xy = 3.1507;              // 3.1507 counts per uT using the +-1300 uT range from the XY axis (13-bit)
const float mag_counts_per_ut_z = 6.5536;               // 6.5536 counts per uT using the +-2500 uT range from the XY axis (15-bit)

esp_err_t imu_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    sample_mutex = xSemaphoreCreateMutexStatic(&sample_mutex_buffer);

    esp_err_t err = ESP_OK;
    uint8_t id;

    // Setup accelerometer, 4g range, 125 Hz filter, normal mode
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_RANGE_REG, 0x05);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_BW_REG, 0x0C);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_PMU_LPW, 0x00);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring accelerometer");
        return ESP_FAIL;
    }
    // Read accelerometer ID
    i2c_manager_read_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_ID_REG, &id);
    ESP_LOGV(TAG, "Accelerometer ID: %d", id);
    if(id != IMU_MANAGER_ACC_ID) 
    {
        ESP_LOGE(TAG, "Error reading accelerometer ID");
        return ESP_FAIL;
    }

    // Setup gyroscope, 2000 degrees/s scale, 100 Hz sample rate, normal mode
    err = ESP_OK;
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_RANGE_REG, 0x00);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_BW_REG, 0x07);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_LPM1_REG, 0x00);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring gyroscope");
        return ESP_FAIL;
    }
    // Read gyroscope ID
    i2c_manager_read_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_ID_REG, &id);
    ESP_LOGV(TAG, "Gyroscope ID: %d", id);
    if(id != IMU_MANAGER_GYRO_ID) 
    {
        ESP_LOGE(TAG, "Error reading gyroscope ID");
        return ESP_FAIL;
    }

    // Setup magnetometer, soft reset, 30 Hz sample rate, enable all axis (x,y and z), 4 repetitions x-y axis, 15 repetitions z axis
    err = ESP_OK;
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_PWRCTRL_REG, 0x82);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_PWRCTRL_REG, 0x01);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_OP_MODE_REG, 0x38);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_BASE_3_REG, 0x84);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_REP_XY_REG, 0x07);
    err += i2c_manager_write_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_REP_Z_REG, 0x22);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring magnetometer");
        return ESP_FAIL;
    }
    // Read magnetometer ID
    i2c_manager_read_register(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS, IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_ID_REG, &id);
    ESP_LOGV(TAG, "Magnetometer ID: %d", id);
    if(id != IMU_MANAGER_MAG_ID) 
    {
        ESP_LOGE(TAG, "Error reading magnetometer ID");
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
            IMU_MANAGER_ACCELEROMETER_ADDRESS, IMU_MANAGER_ACC_DATA_REG, 6, acc)) != ESP_OK)
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
            IMU_MANAGER_GYROSCOPE_ADDRESS, IMU_MANAGER_GYRO_DATA_REG, 6, gyro)) != ESP_OK)
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
            IMU_MANAGER_MAGNETOMETER_ADDRESS, IMU_MANAGER_MAG_DATA_REG, 6, mag)) != ESP_OK)
            {
                ESP_LOGE(TAG, "Error reading magnetometer: %d", err);
                xSemaphoreGive(sample_mutex);
                return err;
            }
        ESP_LOGV(TAG, "Mag array: %d, %d, %d, %d, %d, %d", mag[0], mag[1], mag[2], mag[3], mag[4], mag[5]);
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

/**
 * @brief Get acceleration values from the accelerometer in milli-g
 * @return imu_axis_data_f_t 
 */
imu_axis_data_f_t imu_manager_get_acceleration(void)
{
    // Calculate the acceleration in mg
    imu_axis_data_f_t data = 
    {
        .x = ((float)acc_data.x / counts_per_g) * 1000.0,
        .y = ((float)acc_data.y / counts_per_g) * 1000.0,
        .z = ((float)acc_data.z / counts_per_g) * 1000.0
    };

    ESP_LOGD(TAG, "Accelerometer mg (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}
imu_axis_data_t imu_manager_get_acceleration_raw(void)
{
    return acc_data;
}

/**
 * @brief Get angular velocity from the gyroscope in degrees/s
 * @return imu_axis_data_f_t 
 */
imu_axis_data_f_t imu_manager_get_gyro(void)
{
    // Calculate angular velocity in degrees/s
    imu_axis_data_f_t data = 
    {
        .x = (float)gyro_data.x / gyro_countsper_deg_per_sec,
        .y = (float)gyro_data.y / gyro_countsper_deg_per_sec,
        .z = (float)gyro_data.z / gyro_countsper_deg_per_sec
    };

    ESP_LOGD(TAG, "Gyroscope degrees (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}
imu_axis_data_t imu_manager_get_gyro_raw(void)
{
    return gyro_data;
}

/**
 * @brief Get magnetic field strength from the magnetometer in uT
 * @return imu_axis_data_f_t 
 */
imu_axis_data_f_t imu_manager_get_magnetometer(void)
{
    // Calculate magnetic field in uT
    imu_axis_data_f_t data = 
    {
        .x = (float)mag_data.x / mag_counts_per_ut_xy,
        .y = (float)mag_data.y / mag_counts_per_ut_xy,
        .z = (float)mag_data.x / mag_counts_per_ut_z,
    };

    ESP_LOGD(TAG, "Magnetometer uT (x,y,z): %.2f,%.2f,%.2f", data.x, data.y, data.z);

    return data;
}
imu_axis_data_t imu_manager_get_mag_raw(void)
{
    return mag_data;
}
