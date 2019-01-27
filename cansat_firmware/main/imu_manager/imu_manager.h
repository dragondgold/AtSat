#ifndef __IMU_MANAGER_H__
#define __IMU_MANAGER_H__

#include "esp_system.h"

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} imu_axis_data_t;

typedef struct {
    float x;
    float y;
    float z;
} imu_axis_data_f_t;

#define IMU_MANAGER_ACCELEROMETER_ADDRESS       (0x18 << 1)
#define IMU_MANAGER_GYROSCOPE_ADDRESS           (0x68 << 1)
#define IMU_MANAGER_MAGNETOMETER_ADDRESS        (0x10 << 1)

// Accelerometer
#define IMU_MANAGER_ACC_PMU_RANGE_REG           0x0F
#define IMU_MANAGER_ACC_PMU_BW_REG              0x10
#define IMU_MANAGER_ACC_PMU_LPW                 0x11

#define IMU_MANAGER_ACC_DATA_REG                0x02

// Gyroscope
#define IMU_MANAGER_GYRO_RANGE_REG              0x0F
#define IMU_MANAGER_GYRO_BW_REG                 0x10
#define IMU_MANAGER_GYRO_LPM1                   0x11

#define IMU_MANAGER_GYRO_DATA_REG               0x02

// Magnetometer
#define IMU_MANAGER_MAG_BASE_REG                0x4B
#define IMU_MANAGER_MAG_BASE_2_REG              0x4C
#define IMU_MANAGER_MAG_BASE_3_REG              0x4E
#define IMU_MANAGER_MAG_BASE_4_REG              0x51
#define IMU_MANAGER_MAG_BASE_5_REG              0x52

#define IMU_MANAGER_MAG_DATA_REG                0x42

esp_err_t imu_manager_init(void);
esp_err_t imu_manager_sample_all(void);

imu_axis_data_f_t imu_manager_get_acceleration(void);
imu_axis_data_t imu_manager_get_acceleration_raw(void);
imu_axis_data_f_t imu_manager_get_gyro(void);
imu_axis_data_t imu_manager_get_gyro_raw(void);
imu_axis_data_f_t imu_manager_get_magnetometer(void);

#endif // __IMU_MANAGER_H__
