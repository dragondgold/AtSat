#include "sup_cmd_manager.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"

static const char* TAG = "sup_cmd";

esp_err_t sup_cmd_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    ESP_LOGI(TAG, "Ready");

    return ESP_OK;
}
