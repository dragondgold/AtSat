#include "pressure_manager.h"

#include "config/cansat.h"
#include "i2c_manager/i2c_manager.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char* TAG = "pressure";

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

static float pressure, temperature;
static uint32_t coefficients[6];

static esp_err_t send_cmd(uint8_t command)
{
    esp_err_t err;

    // Acquire the I2C module
    if((err = i2c_manager_acquire(GENERAL_I2C_NUMBER, 500 / portTICK_PERIOD_MS)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error acquiring bus");
        return err;
    }

    // Create the commands
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PRESSURE_MANAGER_ADDRESS, true);
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);

    // Send everything, this will block until everything is sent
    err = i2c_master_cmd_begin(GENERAL_I2C_NUMBER, cmd, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // Release the bus
    i2c_manager_release(GENERAL_I2C_NUMBER);
    return err;
}

static esp_err_t read_coefficients()
{
    esp_err_t err;

    // Read the 6 coefficients from the sensor
    for(unsigned int n = 0; n < 6; ++n) {
        ESP_LOGV(TAG, "Reading coefficient %u", n);
        unsigned int retries = 0;
        uint8_t data[2] = {0};
        
        // Retry until no error
        do {
            // Delay between retries
            vTaskDelay(50 / portTICK_PERIOD_MS);
            
            // Read the two bytes from the coefficient
            err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS,
             PRESSURE_MANAGER_ADDRESS, PRESSURE_MANAGER_BASE_CAL_ADDRESS + (2*n), 2, data);

        } while((err != ESP_OK || (data[0] == 0x00 && data[1] == 0x00)) && retries++ < 10);
        
        if(retries >= 10) {
            ESP_LOGE(TAG, "Error reading coefficient %u", n);
            return ESP_FAIL;
        }
        
        // Combine the two bytes, MSB is first.
        coefficients[n] = (((uint16_t)data[0]) << 8) | data[1];
    }

    return ESP_OK;
}

/**
 * @brief Driver for the MS5637-02BA03 pressure sensor
 * @return esp_err_t 
 */
esp_err_t pressure_manager_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Reset the sensor
    ESP_LOGV(TAG, "Restarting sensor");
    if((err = send_cmd(PRESSURE_MANAGER_RESET_CMD)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error restarting sensor in init: %s", esp_err_to_name(err));
        return err;
    }

    // Read coefficients
    ESP_LOGV(TAG, "Reading coefficients");
    if((err = read_coefficients()) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error reading coefficients: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGV(TAG, "Coefficients are: %d,%d,%d,%d,%d,%d", coefficients[0], coefficients[1],
     coefficients[2], coefficients[3], coefficients[4], coefficients[5]);

    // Do the first sample
    pressure_manager_do_sample();
    
    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}

esp_err_t pressure_manager_do_sample(void)
{
    ESP_LOGV(TAG, "Starting sample");
    if(xSemaphoreTake(mutex, 100 / portTICK_PERIOD_MS))
    {
        esp_err_t err;

        // Reset the sensor before each sample just in case it hangs or something
        if((err = send_cmd(PRESSURE_MANAGER_RESET_CMD)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error restarting sensor %s:", esp_err_to_name(err));
            pressure = temperature = 0;
            xSemaphoreGive(mutex);
            return err;
        }

        // Sample pressure
        if((err = send_cmd(PRESSURE_MANAGER_PRESSURE_OSR_512_CMD)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error sampling pressure: %s", esp_err_to_name(err));
            pressure = temperature = 0;
            xSemaphoreGive(mutex);
            return err;
        }
        // Wait some time for the pressure to be sampled
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // Read the 3 bytes from the ADC now
        uint8_t buffer_pressure[3] = {0};
        if((err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS,
            PRESSURE_MANAGER_ADDRESS, PRESSURE_MANAGER_ADC_RESULT_CMD, 3, buffer_pressure)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error reading pressure ADC: %s", esp_err_to_name(err));
            pressure = temperature = 0;
            xSemaphoreGive(mutex);
            return err;
        }

        // Sample the temperature now
        if((err = send_cmd(PRESSURE_MANAGER_TEMP_OSR_512_CMD)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error sampling temperature: %s", esp_err_to_name(err));
            pressure = temperature = 0;
            xSemaphoreGive(mutex);
            return err;
        }
        // Wait some time for the temperature to be sampled
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // Read the 3 bytes from the ADC now
        uint8_t buffer_temp[3] = {0};
        if((err = i2c_manager_read_register_multiple(GENERAL_I2C_NUMBER, 100 / portTICK_PERIOD_MS,
            PRESSURE_MANAGER_ADDRESS, PRESSURE_MANAGER_ADC_RESULT_CMD, 3, buffer_temp)) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error reading temperature ADC: %s", esp_err_to_name(err));
            pressure = temperature = 0;
            xSemaphoreGive(mutex);
            return err;
        }

        // Compute the pressure compensated with temperature
        int32_t dT, TEMP;
        int64_t OFF, SENS, P, T2, OFF2, SENS2;
        
        uint32_t adc_temperature = (((uint32_t)buffer_temp[0]) << 16) | (((uint32_t)buffer_temp[1]) << 8) | buffer_temp[2];
        uint32_t adc_pressure = (((uint32_t)buffer_pressure[0]) << 16) | (((uint32_t)buffer_pressure[1]) << 8) | buffer_pressure[2];
        
        // Difference between actual and reference temperature = D2 - Tref
        dT = (int32_t)adc_temperature - ((int32_t)coefficients[4] <<8 );
        
        // Actual temperature = 2000 + dT * TEMPSENS
        TEMP = 2000 + ((int64_t)dT * (int64_t)coefficients[5] >> 23) ;
        
        // Second order temperature compensation
        if(TEMP < 2000) {
            T2 = ( 3 * ( (int64_t)dT  * (int64_t)dT  ) ) >> 33;
            OFF2 = 61 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 16 ;
            SENS2 = 29 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 16 ;

            if( TEMP < -1500 )
            {
                OFF2 += 17 * ((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500) ;
                SENS2 += 9 * ((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500) ;
            }
        }
        else {
            T2 = ( 5 * ( (int64_t)dT  * (int64_t)dT  ) ) >> 38;
            OFF2 = 0 ;
            SENS2 = 0 ;
        }
        
        // OFF = OFF_T1 + TCO * dT
        OFF = ( (int64_t)(coefficients[1]) << 17 ) + ( ( (int64_t)(coefficients[3]) * dT ) >> 6 ) ;
        OFF -= OFF2 ;

        // Sensitivity at actual temperature = SENS_T1 + TCS * dT
        SENS = ( (int64_t)coefficients[0] << 16 ) + ( ((int64_t)coefficients[2] * dT) >> 7 ) ;
        SENS -= SENS2 ;

        // Temperature compensated pressure = D1 * SENS - OFF
        P = ( ( (adc_pressure * SENS) >> 21 ) - OFF ) >> 15;
        
        temperature = ( (float)TEMP - T2 ) / 100.0;
        pressure = (float)P / 100.0;
        ESP_LOGV(TAG, "Temp: %.2f. Pressure: %.2f", temperature, pressure);

        xSemaphoreGive(mutex);
        return ESP_OK;
    }
    ESP_LOGE(TAG, "Error getting sampling mutex");

    return ESP_FAIL;
}

float pressure_manager_get_pressure(void)
{
    return pressure;
}

float pressure_manager_get_temperature(void)
{
    return temperature;
}