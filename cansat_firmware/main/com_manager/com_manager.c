#include "com_manager.h"

#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "libs/cc1101_driver/cc1101.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "spi_manager/spi_manager.h"

#include "esp_log.h"

static const char* TAG = "com";

// Task
static StackType_t stack[GPS_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

static void com_task(void* arg) 
{
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t com_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");
    
    if(!cc1101_init())
    {
        ESP_LOGE(TAG, "Error initializing CC1101 driver");
        return ESP_FAIL;
    }
    if(!cc1101_set_mhz(COM_MANAGER_CARRIER_FREQUENCY))
    {
        ESP_LOGE(TAG, "Error setting CC1101 carrier frequency");
        return ESP_FAIL;
    }

    // Create task
    ESP_LOGV(TAG, "Creating task");
    task_handle = xTaskCreateStaticPinnedToCore(com_task, "com", COM_MANAGER_STACK_SIZE, 
        NULL, COM_MANAGER_TASK_PRIORITY, stack, &task, COM_MANAGER_AFFINITY);

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}