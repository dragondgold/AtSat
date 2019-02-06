/*
 Firmware para el satélite CanSat desarrollado por AXTEC y distribuido
  bajo la licencia GNU GPL V3.0.

  Autor: Andrés Fernando Torti
  Fecha: 21/01/2019
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "config/cansat.h"

#include "aux_ps/aux_ps.h"
#include "i2c_manager/i2c_manager.h"
#include "spi_manager/spi_manager.h"
#include "sensor_manager/sensor_manager.h"
#include "battery_manager/battery_manager.h"
#include "servo_manager/servo_manager.h"
#include "led_manager/led_manager.h"
#include "com_manager/com_manager.h"

#ifdef DEBUG
    #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#endif
#include "esp_log.h"

static const char* TAG = "main";

#ifdef DEBUG
/**
 * @brief Print information about the system. For this to work configUSE_TRACE_FACILITY must be enabled
 *  using 'make menuconfig' then selecting 'Component config->FreeRTOS->Enable FreeRTOS trace facility'
 */
void vGetRunTimeStats(TimerHandle_t xTimer)
{
    ESP_LOGD(TAG, "Getting runtime data");

    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;

    // Take a snapshot of the number of tasks in case it changes while this
    // function is executing.
    uxArraySize = uxTaskGetNumberOfTasks();
    ESP_LOGD(TAG, "%d tasks running", uxArraySize);

    // Allocate a TaskStatus_t structure for each task.  An array could be
    // allocated statically at compile time.
    pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if(pxTaskStatusArray != NULL)
    {
        // Generate raw status information about each task.
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

        // For each populated position in the pxTaskStatusArray array,
        // format the raw data as human readable ASCII data
        for( x = 0; x < uxArraySize; x++ )
        {
            ESP_LOGD(TAG, "------ Task \"%s\"", pxTaskStatusArray[x].pcTaskName);
            ESP_LOGD(TAG, "Free stack: %d bytes", pxTaskStatusArray[x].usStackHighWaterMark);
        }

        // The array is no longer needed, free the memory it consumes.
        vPortFree(pxTaskStatusArray);
    }
    else
    {
        ESP_LOGD(TAG, "------ pxTaskStatusArray is NULL");
    }

    ESP_LOGD(TAG, "------ Finished vGetRunTimeStats()");
}
#endif

void app_main()
{
    ESP_LOGI(TAG, "System boot!");

    // Get chip information and output it
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "ESP32 with %d CPU cores. Revision %d.",
            chip_info.cores,
            chip_info.revision);

    // Flash information
    ESP_LOGI(TAG, "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    fflush(stdout);

    // Init every system in the satellite
    esp_err_t err = ESP_OK;
    ESP_LOGI(TAG, "Initializing systems");

    // Install gpio isr service
    gpio_install_isr_service(0);

    // Give some time to circuits to power-up
    vTaskDelay(pdMS_TO_TICKS(500));

    err += led_manager_init();
    led_manager_fast_blink(true);

    err += aux_ps_init();
    err += i2c_manager_init();
    err += spi_manager_init();
    //err += sensor_manager_init();
    err += battery_manager_init();
    err += servo_manager_init();
    err += com_manager_init();

    ESP_LOGI(TAG, "RAM left %d bytes", esp_get_free_heap_size());

    if(err == ESP_OK)
    {
        ESP_LOGI(TAG, "Systems ready!");
        led_manager_on();
    }
    else
    {
        ESP_LOGE(TAG, "System init failed!");
        led_manager_fast_blink(false);
    }

// When debug is defined, print the system status after 3 seconds running
#ifdef DEBUG
    if(xTimerStart((int)xTimerCreate("stats_tmr", pdMS_TO_TICKS(3000), pdFALSE, (void *)0, vGetRunTimeStats), 500 / portTICK_PERIOD_MS) == pdFAIL)
    {
        ESP_LOGE(TAG, "Error starting timer");
    }
#endif
}
