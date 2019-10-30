#ifndef __SENSOR_MANAGER_H__
#define __SENSOR_MANAGER_H__

#include "esp_system.h"
#include "imu_manager/imu_manager.h"
#include "minmea/minmea.h"

typedef struct
{
    int64_t timestamp;              // Timestamp in us
    bool valid;

    // Gyroscope, accelerometer and magnetometer data
    imu_axis_data_f_t gyro;         // degrees/s
    imu_axis_data_f_t acc;          // mg
    imu_axis_data_f_t mag;          // uT
    imu_axis_data_f_t orientation;  // degrees

    float pressure;                 // mBar
    float temperature;              // degrees celsius
    uint8_t humidity;               // %

    struct minmea_float latitude;
    struct minmea_float longitude;
    float altitude;                 // Altitude in meters
    bool gps_fix;
} sensors_data_t;

#define SENSOR_MANAGER_FALLING_THRESHOLD    50  // meters
#define SENSOR_MANAGER_ENABLE_SAMPLE_STORE  true
#define SENSOR_MANAGER_SAMPLE_ITEM_SIZE     sizeof(sensors_data_t)

#if SENSOR_MANAGER_ENABLE_SAMPLE_STORE
    #define SENSOR_MANAGER_MAX_SAMPLES_TO_STORE 1024
#else
    #define SENSOR_MANAGER_MAX_SAMPLES_TO_STORE 1
#endif

esp_err_t sensor_manager_init(void);

/**
 * @brief Get the data from all the main sensors in the system
 * 
 * @param data struct to save the sensors data
 * @return true if data was saved
 * @return false couldn't save the data
 */
bool sensor_manager_get_data(sensors_data_t* data);

/**
 * @brief Get all the samples currently stored
 * 
 * @return sensors_data_t* array of size SENSOR_MANAGER_MAX_SAMPLES_TO_STORE containing
 *  all the samples
 */
sensors_data_t* sensor_manager_get_samples(void);

#endif // __SENSOR_MANAGER_H__
