#include "sensor_manager.h"

#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "libs/sensor_fusion/sensor_fusion.h"

#include "gps_manager/gps_manager.h"
#include "imu_manager/imu_manager.h"
#include "pressure_manager/pressure_manager.h"
#include "temp_hum_manager/temp_hum_manager.h"

static const char* TAG = "sensor";

// Sensor fusion
static SensorFusionGlobals sfg;                 // Sensor fusion instance
static struct PhysicalSensor sensors[3];        // Storage for the sensors used
static registerDeviceInfo_t i2cBusInfo = {      // It doesn't matter, this is not used in our case
    .deviceInstance     = 0,
    .functionParam      = NULL,
    .idleFunction       = NULL
};
static int16_t accCountsPerG = 1024;            // 1024 counts per g as the accelerometer is in the 4g range
static int16_t gyroCountsPerDegPerSec = 131;    // 131 counts per degree/s as the gyro is in the 250 degree/s range
static int16_t magCountsPeruT = 6;              // 6 counts per uT using the 1300 uT range from the XY axis

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

        ESP_LOGV(TAG, "Roll: %.2f, %.2f, %.2f", sfg.SV_9DOF_GBY_KALMAN.fPhiPl, sfg.SV_9DOF_GBY_KALMAN.fThePl, sfg.SV_9DOF_GBY_KALMAN.fPsiPl);

        // Sample at FUSION_HZ rate
        #if FUSION_HZ > 100
            #error FUSION_HZ cannot be higher than the RTOS tick rate
        #endif
        vTaskDelay(((1.0F/(float)FUSION_HZ)*1000) / portTICK_PERIOD_MS);
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
    // Magnetometer init
    else if(sensor->addr == IMU_MANAGER_MAGNETOMETER_ADDRESS)
    {
        ESP_LOGV(TAG, "Init mag");
        sfg->Mag.iWhoAmI = 62;          // Dummy value (should be something like the sensor ID)
        sfg->Mag.iCountsPeruT = magCountsPeruT;
        sfg->Mag.fCountsPeruT = 1.0F / (float)magCountsPeruT;

        sfg->Mag.iFIFOCount = 0;
        sensor->isInitialized = F_USING_MAG;
        sfg->Mag.isEnabled = true;
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

static int8_t read_mag(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    imu_axis_data_t raw = imu_manager_get_mag_raw();
    int16_t data[3] = { raw.x, raw.y, raw.z };

    // Condition the sample and add it to the FIFO for the sensor fusion
    conditionSample(data);
    addToFifo((union FifoSensor*) &(sfg->Mag), MAG_FIFO_SIZE, data);

    return SENSOR_ERROR_NONE;
}

esp_err_t sensor_manager_init(void)
{
    esp_err_t err = ESP_OK;

    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    sample_mutex = xSemaphoreCreateMutexStatic(&sample_mutex_buffer);

    // Init sensors
    err += gps_manager_init();
    err += pressure_manager_init();
    err += temp_hum_manager_init();
    err += imu_manager_init();

    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting sensor manager");
        return err;
    }

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
    sfg.installSensor(&sfg, &sensors[2], IMU_MANAGER_MAGNETOMETER_ADDRESS, 1, NULL, &i2cBusInfo, dummy_sensor_init, read_mag);

    // Initialize sensors
    sfg.initializeFusionEngine(&sfg);

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}