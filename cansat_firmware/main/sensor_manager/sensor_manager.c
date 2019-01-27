#include "sensor_manager.h"

#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#define LOG_LOCAL_LEVEL     ESP_LOG_VERBOSE
#include "esp_log.h"
#include "libs/sensor_fusion/sensor_fusion.h"

#include "gps_manager/gps_manager.h"
#include "imu_manager/imu_manager.h"
#include "pressure_manager/pressure_manager.h"
#include "temp_hum_manager/temp_hum_manager.h"

static const char* TAG = "sensor";

// Sensor fusion
static SensorFusionGlobals sfg;
static struct PhysicalSensor sensors[2];
static registerDeviceInfo_t i2cBusInfo = {
    .deviceInstance     = 0,
    .functionParam      = NULL,
    .idleFunction       = NULL
};
static int16_t accCountsPerG = 1024;            // 1024 counts per g as the accelerometer is in the 4g range
static int16_t gyroCountsPerDegPerSec = 218;    // 218 counts per degree/s as the gyro is in the 150 degree/s range 

// Mutex
static StaticSemaphore_t sample_mutex_buffer;
static SemaphoreHandle_t sample_mutex;

// Task
static StackType_t stack[SENSOR_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

static void sensors_sample_task(void* arg) 
{
    while(true)
    {
        imu_manager_sample_all();

        sfg.readSensors(&sfg, 1);           
        sfg.conditionSensorReadings(&sfg);  
        sfg.runFusion(&sfg);
        sfg.loopcounter++;

        ESP_LOGI(TAG, "Roll: %.2f, %.2f, %.2f", sfg.SV_6DOF_GY_KALMAN.fPhiPl, sfg.SV_6DOF_GY_KALMAN.fThePl, sfg.SV_6DOF_GY_KALMAN.fPsiPl);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Dummy function for sensors init for the NXP Sensor Function library
 * @param sensor 
 * @param sfg 
 * @return int8_t 
 */
static int8_t dummy_sensor_init(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    ESP_LOGV(TAG, "Sensor init");

    // Accelerometer init
    if(sensor->addr == IMU_MANAGER_ACCELEROMETER_ADDRESS)
    {
        ESP_LOGV(TAG, "Init acc");
        sfg->Accel.iWhoAmI = 60;        // Dummy value (should be something like the sensor ID)
        sfg->Accel.iCountsPerg = accCountsPerG;  
        sfg->Accel.fgPerCount = 1.0F / (float)accCountsPerG;

        sfg->Accel.iFIFOCount = 0;
        sensor->isInitialized = F_USING_ACCEL;
        sfg->Accel.isEnabled = true;
    }
    // Gyroscope init
    else if(sensor->addr == IMU_MANAGER_GYROSCOPE_ADDRESS)
    {
        ESP_LOGV(TAG, "Init gyro");
        sfg->Gyro.iWhoAmI = 61;         // Dummy value (should be something like the sensor ID)
        sfg->Gyro.iCountsPerDegPerSec = gyroCountsPerDegPerSec;
        sfg->Gyro.fDegPerSecPerCount = 1.0F / (float)gyroCountsPerDegPerSec;

        sfg->Gyro.iFIFOCount = 0;
        sensor->isInitialized = F_USING_GYRO;
        sfg->Gyro.isEnabled = true;
    }

    return 0;
}

static int8_t read_acc(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    imu_axis_data_t raw = imu_manager_get_acceleration_raw();
    int16_t data[3] = { raw.x, raw.y, raw.z };

    // Condition the sample and add it to the FIFO for the sensor fusion
    conditionSample(data);
    addToFifo((union FifoSensor*) &(sfg->Accel), ACCEL_FIFO_SIZE, data);

    return SENSOR_ERROR_NONE;
}

static int8_t read_gyro(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    imu_axis_data_t raw = imu_manager_get_gyro_raw();
    int16_t data[3] = { raw.x, raw.y, raw.z };

    // Condition the sample and add it to the FIFO for the sensor fusion
    conditionSample(data);
    addToFifo((union FifoSensor*) &(sfg->Gyro), GYRO_FIFO_SIZE, data);

    return SENSOR_ERROR_NONE;
}

esp_err_t sensor_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    sample_mutex = xSemaphoreCreateMutexStatic(&sample_mutex_buffer);

    // Init sensors
    gps_manager_init();
    pressure_manager_init();
    temp_hum_manager_init();
    imu_manager_init();

    // Task to sample sensors
    task_handle = xTaskCreateStaticPinnedToCore(sensors_sample_task, "sensors", SENSOR_MANAGER_STACK_SIZE, 
        NULL, SENSOR_MANAGER_TASK_PRIORITY, stack, &task, SENSOR_MANAGER_AFFINITY);

    // Initialize sensor fusion. The sensor fusion library is the version 7.2 provided by NXP and modified
    //  so it can work standalone in other microcontrollers and outside their framework. The modifications
    //  are not perfect but they work for this case. The library was downloaded from:
    // https://www.nxp.com/support/developer-resources/software-development-tools/sensor-developer-resources/nxp-sensor-fusion:XTRSICSNSTLBOXX
    initSensorFusionGlobals(&sfg, NULL, NULL);

    // Install sensors
    sfg.installSensor(&sfg, &sensors[0], IMU_MANAGER_ACCELEROMETER_ADDRESS, 1, NULL, &i2cBusInfo, dummy_sensor_init, read_acc);
    sfg.installSensor(&sfg, &sensors[1], IMU_MANAGER_GYROSCOPE_ADDRESS, 1, NULL, &i2cBusInfo, dummy_sensor_init, read_gyro);

    // Initialize sensors
    sfg.initializeFusionEngine(&sfg);

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}