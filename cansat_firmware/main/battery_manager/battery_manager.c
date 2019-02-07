#include "battery_manager.h"
#include "libs/bq27441_driver/bq27441.h"
#include "driver/gpio.h"
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"

static const char* TAG = "battery";

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

// Task
static StackType_t stack[BATTERY_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

// Battery data
battery_data_t battery_data = {0};

static void battery_sample_task(void* args)
{
    battery_data_t temp_data;

    while(1)
    {
        // Check battery and charger status every 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGV(TAG, "Checking battery");

        // Inform on charger connect/disconnect
        if(battery_data.is_charging != !gpio_get_level(CHARGING_DETECTION_PIN))
        {
            ESP_LOGI(TAG, "Battery %s", (!gpio_get_level(CHARGING_DETECTION_PIN)) ? "started charging" : "stopped charging");
        }

        // Read all the data from the fuel-gauge
        temp_data.soc = bq27441_get_soc(FILTERED);
        temp_data.volts = bq27441_get_voltage();
        temp_data.avg_current = bq27441_get_current(AVG);
        temp_data.total_capacity = bq27441_get_capacity(FULL);
        temp_data.remaining_capacity = bq27441_get_capacity(REMAIN);
        temp_data.health = bq27441_get_soh(PERCENT);
        temp_data.is_charging = !gpio_get_level(CHARGING_DETECTION_PIN);

        ESP_LOGV(TAG, "Battery SOC: %d %%", battery_data.soc);
        ESP_LOGV(TAG, "Battery voltage: %d mV", battery_data.volts);
        ESP_LOGV(TAG, "Battery avg current: %d mA", battery_data.avg_current);
        ESP_LOGV(TAG, "Battery total capacity: %d mAh", battery_data.total_capacity);
        ESP_LOGV(TAG, "Battery remaining capacity: %d mAh", battery_data.remaining_capacity);
        ESP_LOGV(TAG, "Battery health %d %%", battery_data.remaining_capacity);
        ESP_LOGV(TAG, "Battery is %s", battery_data.is_charging ? "charging" : "not charging");

        if(temp_data.soc != battery_data.soc)
        {
            ESP_LOGI(TAG, "Battery SOC: %d %%", temp_data.soc);
        }

        // Take the mutex so other tasks cannot read the data while it's being
        //  modified
        if(xSemaphoreTake(mutex, 1000 / portTICK_PERIOD_MS))
        {
            // Copy all the data
            battery_data = temp_data;

#if BATTERY_MANAGER_OVERRIDE_CURRENT == true
            // When not charging (USB disconnected), set the charging current at minimum
            if(!battery_data.is_charging)
            {
                gpio_set_direction(CHARGER_ISET_PIN, GPIO_MODE_DEF_INPUT);
                battery_data.charging_at_max = false;
            }

            // Charger detected? Use current defined by the ISET resistor (300 mA)
            if(gpio_get_level(CHARGER_DETECTION_PIN))
            {
                gpio_set_direction(CHARGER_ISET_PIN, GPIO_MODE_DEF_OUTPUT);
                gpio_set_level(CHARGER_ISET_PIN, 0);
                battery_data.charging_at_max = true;
                ESP_LOGV(TAG, "Charging current 300 mA");
            }
            else
            {
                // Charge at minimum current (100 mA)
                gpio_set_direction(CHARGER_ISET_PIN, GPIO_MODE_DEF_INPUT);
                battery_data.charging_at_max = false;
                ESP_LOGV(TAG, "Charging current 100 mA");
            }
#else
            // Set max charging current
            gpio_set_direction(CHARGER_ISET_PIN, GPIO_MODE_DEF_OUTPUT);
            gpio_set_level(CHARGER_ISET_PIN, 0);
            battery_data.charging_at_max = true;
            ESP_LOGV(TAG, "Charging current 300 mA");
#endif
            
            xSemaphoreGive(mutex);
        }
        else 
        {
            ESP_LOGE(TAG, "Error getting mutex on battery_sample_task()");
        }
    }
}

esp_err_t battery_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    // Create mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);
    
    for(unsigned int n = 0; n < 10; ++n)
    {
        ESP_LOGI(TAG, "Initializing %d/10", n+1);
        if(bq27441_init())
        {
            // Enter configuration mode
            ESP_LOGV(TAG, "Entering configuration mode");
            if(bq27441_enter_config(true))
            {
                // Capacity
                ESP_LOGV(TAG, "Setting capacity");
                if(!bq27441_set_capacity(BATTERY_MANAGER_BATTERY_MAH))
                {
                    ESP_LOGE(TAG, "Error setting battery capacity");
                    continue;
                }
                // GPOUT polarity
                ESP_LOGV(TAG, "Setting GPOUT polarity");
                if(!bq27441_set_gpout_polarity(false))
                {
                    ESP_LOGE(TAG, "Error setting GPOUT polarity");
                    continue;
                }
                // GPOUT function
                ESP_LOGV(TAG, "Setting GPOUT function");
                if(!bq27441_set_gpout_function(BAT_LOW))
                {
                    ESP_LOGE(TAG, "Error setting GPOUT function");
                    continue;
                }

                // Thresholds for battery low
                ESP_LOGV(TAG, "Setting low threshold");
                if(!bq27441_set_soc1_thresholds(BATTERY_MANAGER_SOC1_SET_THRESHOLD, BATTERY_MANAGER_SOC1_CLEAR_THRESHOLD))
                {
                    ESP_LOGE(TAG, "Error setting SOC1 thresholds");
                    continue;
                }
                ESP_LOGV(TAG, "Setting high threshold");
                if(!bq27441_set_socf_thresholds(BATTERY_MANAGER_SOCF_SET_THRESHOLD, BATTERY_MANAGER_SOCF_CLEAR_THRESHOLD))
                {
                    ESP_LOGE(TAG, "Error setting SOCF thresholds");
                    continue;
                }

                ESP_LOGV(TAG, "Exitting config mode");
                if(!bq27441_exit_config(true))
                {
                    ESP_LOGE(TAG, "Error exitting config");
                    continue;
                }

                esp_err_t err;

                // Set GPOUT, CHARGING_DETECTION_PIN and CHARGER_DETECTION_PIN pins as input.
                // Currently we are not using GPOUT pin at all.
                ESP_LOGV(TAG, "Configuring pins");
                gpio_config_t config;
                config.pin_bit_mask = (1ULL << GPOUT_PIN) | (1ULL << CHARGING_DETECTION_PIN) | (1ULL << CHARGER_DETECTION_PIN);
                config.mode = GPIO_MODE_DEF_INPUT;
                config.pull_up_en = GPIO_PULLUP_DISABLE;
                config.pull_down_en = GPIO_PULLDOWN_DISABLE;
                config.intr_type = GPIO_INTR_DISABLE;
                if((err = gpio_config(&config)) != ESP_OK)
                {
                    return err;
                }

                // Set ISET pin as input at start so the charging current is 100 mA
                config.pin_bit_mask = (1ULL << CHARGER_ISET_PIN);
                config.mode = GPIO_MODE_DEF_INPUT;
                config.pull_up_en = GPIO_PULLUP_DISABLE;
                config.pull_down_en = GPIO_PULLDOWN_DISABLE;
                config.intr_type = GPIO_INTR_DISABLE;
                if((err = gpio_config(&config)) != ESP_OK)
                {
                    return err;
                }

                // Task to sample sensors
                ESP_LOGV(TAG, "Starting task");
                task_handle = xTaskCreateStaticPinnedToCore(battery_sample_task, "battery", BATTERY_MANAGER_STACK_SIZE, 
                    NULL, BATTERY_MANAGER_TASK_PRIORITY, stack, &task, BATTERY_MANAGER_AFFINITY);

                ESP_LOGI(TAG, "Ready!");
                return ESP_OK;
            }
            else 
            {
                ESP_LOGE(TAG, "Error entering configuration mode");
            }

            // Wait a bit between retries
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    ESP_LOGE(TAG, "Error initializing!");
    return ESP_FAIL;
}

battery_data_t battery_manager_get(void)
{
    battery_data_t data = {0};
    // Take the mutex so the struct cannot be changed while returning it
    if(xSemaphoreTake(mutex, 500 / portTICK_PERIOD_MS))
    {
        // Make a copy so we are safe to pass it atomically
        data = battery_data;
        xSemaphoreGive(mutex);
        
        return data;
    }
    
    return data;
}