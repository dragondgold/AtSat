#include "sensor_manager.h"

#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "libs/sensor_fusion/sensor_fusion.h"
#include "libs/minmea/minmea.h"

#include "gps_manager/gps_manager.h"
#include "imu_manager/imu_manager.h"
#include "pressure_manager/pressure_manager.h"
#include "humidity_manager/humidity_manager.h"

#include <string.h>

static const char* TAG = "sensor";

sensors_data_t sensors_data;

// Sensor fusion
static SensorFusionGlobals sfg;                 // Sensor fusion instance
static struct PhysicalSensor sensors[3];        // Storage for the sensors used
static registerDeviceInfo_t i2cBusInfo = {      // It doesn't matter, this is not used in our case
    .deviceInstance     = 0,
    .functionParam      = NULL,
    .idleFunction       = NULL
};
static int16_t accCountsPerG = 512;                 // 512 counts per g as the accelerometer is in the 4g range (12-bit)
static int16_t gyroCountsPerDegPerSec = 16;         // 16 counts per degree/s as the gyro is in the 2000 degree/s range (16-bit)
static int16_t magCountsPeruT = 3;                  // 3 counts per uT using the 1300 uT range from the XY axis (13-bit)
static const float mag_z_scale_factor = 1.92307;    // 2500 uT / 1300 uT. Scaling to make the 2500 uT axis behave as 1300 uT range

// Mutex
static StaticSemaphore_t sample_mutex_buffer;
static SemaphoreHandle_t sample_mutex;

// Task
static StackType_t stack_fusion[SENSOR_MANAGER_FUSION_STACK_SIZE];
static StaticTask_t task_fusion;
static TaskHandle_t task_handle_fusion;

static StackType_t stack_sensors[SENSOR_MANAGER_SENSORS_STACK_SIZE];
static StaticTask_t task_sensors;
static TaskHandle_t task_handle_sensors;

static void fusion_task(void* arg) 
{
    while(true)
    {
        imu_manager_sample_all();

        sfg.readSensors(&sfg, 1);           
        sfg.conditionSensorReadings(&sfg);  
        sfg.runFusion(&sfg);
        sfg.loopcounter++;

        ESP_LOGV(TAG, "Roll: %.2f, %.2f, %.2f", sfg.SV_9DOF_GBY_KALMAN.fPhiPl, sfg.SV_9DOF_GBY_KALMAN.fThePl, sfg.SV_9DOF_GBY_KALMAN.fPsiPl);

        // Update the data
        if(xSemaphoreTake(sample_mutex, 20 / portTICK_PERIOD_MS))
        {
            sensors_data.gyro = imu_manager_get_gyro();
            sensors_data.acc = imu_manager_get_acceleration();
            sensors_data.mag = imu_manager_get_magnetometer();
            sensors_data.orientation.x = sfg.SV_9DOF_GBY_KALMAN.fPhiPl;
            sensors_data.orientation.y = sfg.SV_9DOF_GBY_KALMAN.fThePl;
            sensors_data.orientation.z = sfg.SV_9DOF_GBY_KALMAN.fPsiPl;
            xSemaphoreGive(sample_mutex);
        }

        // Sample at FUSION_HZ rate
        #if FUSION_HZ > configTICK_RATE_HZ
            #error FUSION_HZ cannot be higher than the RTOS tick rate
        #endif
        vTaskDelay(((1.0F/(float)FUSION_HZ)*1000) / portTICK_PERIOD_MS);
    }
}

static void sensors_task(void* arg)
{
    struct minmea_sentence_gga gps_data;

    while(true)
    {
        // Update temperature, pressure, humidity and location every 500 ms
        vTaskDelay(500 / portTICK_PERIOD_MS);

        pressure_manager_do_sample();
        humidity_manager_sample();

        // Update the data
        if(xSemaphoreTake(sample_mutex, 100 / portTICK_PERIOD_MS))
        {
            sensors_data.pressure = pressure_manager_get_pressure();
            sensors_data.temperature = pressure_manager_get_temperature();
            sensors_data.humidity = (uint8_t)humidity_manager_get_humidity();

            gps_data = gps_manager_get_gga();
            sensors_data.latitude = gps_data.latitude;
            sensors_data.longitude = gps_data.longitude;
            sensors_data.altitude = minmea_tofloat(&gps_data.altitude);
            xSemaphoreGive(sample_mutex);
        }
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

    //ESP_LOGV(TAG, "Acc: %d %d %d", raw.x, raw.y, raw.z);

    // Condition the sample and add it to the FIFO for the sensor fusion
    conditionSample(data);
    addToFifo((union FifoSensor*) &(sfg->Accel), ACCEL_FIFO_SIZE, data);

    return SENSOR_ERROR_NONE;
}

static int8_t read_gyro(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    imu_axis_data_t raw = imu_manager_get_gyro_raw();
    int16_t data[3] = { raw.x, raw.y, raw.z };

    //ESP_LOGV(TAG, "Gyro: %d %d %d", raw.x, raw.y, raw.z);

    // Condition the sample and add it to the FIFO for the sensor fusion
    conditionSample(data);
    addToFifo((union FifoSensor*) &(sfg->Gyro), GYRO_FIFO_SIZE, data);

    return SENSOR_ERROR_NONE;
}

static int8_t read_mag(struct PhysicalSensor *sensor, SensorFusionGlobals *sfg)
{
    imu_axis_data_t raw = imu_manager_get_mag_raw();

    // The magnetometer has different scales for the XY and Z axis. XY axis
    //  have a full scale range of 1300 uT and the Z axis has a full scale
    //  range of 2500 uT (typical values).
    // So we have to scale the Z axis to behave as a 1300 uT full range because
    //  the fusion lib expects the same range for all the axis.
    raw.z = (float)raw.z * mag_z_scale_factor;

    // Clip the output
    if(raw.z > 8192)
    {
        raw.z = 8192;
    }
    else if(raw.z < -8192)
    {
        raw.z = -8192;
    }

    int16_t data[3] = { raw.x, raw.y, raw.z };

    //ESP_LOGV(TAG, "Mag: %d %d %d", raw.x, raw.y, raw.z);

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
    err += humidity_manager_init();
    err += imu_manager_init();

    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting sensor manager");
        return err;
    }

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

    // Tasks to sample sensors and run fusion algorithm
    task_handle_fusion = xTaskCreateStaticPinnedToCore(fusion_task, "fusion", SENSOR_MANAGER_FUSION_STACK_SIZE, 
        NULL, SENSOR_MANAGER_FUSION_TASK_PRIORITY, stack_fusion, &task_fusion, SENSOR_MANAGER_FUSION_AFFINITY);
    task_handle_sensors = xTaskCreateStaticPinnedToCore(sensors_task, "sensors", SENSOR_MANAGER_SENSORS_STACK_SIZE, 
        NULL, SENSOR_MANAGER_SENSORS_TASK_PRIORITY, stack_sensors, &task_sensors, SENSOR_MANAGER_SENSORS_AFFINITY);

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}

bool sensor_manager_get_data(sensors_data_t* data)
{
    if(xSemaphoreTake(sample_mutex, 100 / portTICK_PERIOD_MS))
    {
        // Copy the data
        memcpy((void *)data, (void *)(&sensors_data), sizeof(sensors_data_t));
        xSemaphoreGive(sample_mutex);
        
        return true;
    }

    return false;
}