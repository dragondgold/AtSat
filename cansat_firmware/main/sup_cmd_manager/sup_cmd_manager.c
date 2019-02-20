#include "sup_cmd_manager.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "config/cansat.h"

#include "esp_log.h"

#define LOG_LOCAL_LEVEL     ESP_LOG_VERBOSE
static const char* TAG = "sup_cmd";

// Task
static StackType_t stack[SUP_CMD_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

// UART queue
static QueueHandle_t uart_queue;
static uint8_t buffer[SUP_CMD_MANAGER_UART_BUFFER_SIZE] = {0};

static void sup_cmd_task(void* arg)
{
    static uart_event_t event;

    while(true)
    {
        if(xQueueReceive(uart_queue, &event, portMAX_DELAY)) 
        {
            switch(event.type) 
            {
                case UART_PATTERN_DET:
                    {   
                        int pos = uart_pattern_pop_pos(ARDUINO_UART_NUMBER);

                        ESP_LOGV(TAG, "[UART PATTERN DETECTED] pos: %d", pos);
                        
                        if (pos == -1) 
                        {
                            // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                            // record the position. We should set a larger queue size if we get here.
                            ESP_LOGE(TAG, "Pattern fail");
                            uart_flush_input(ARDUINO_UART_NUMBER);
                        } 
                        else 
                        {
                            // Read all the bytes up to the position where the pattern was detected
                            uart_read_bytes(ARDUINO_UART_NUMBER, buffer, pos, pdMS_TO_TICKS(100));

                            uint8_t pattern[2] = { 0 };
                            uart_read_bytes(ARDUINO_UART_NUMBER, pattern, 1, pdMS_TO_TICKS(100));

                            //ESP_LOGI(TAG, "read data: %s", dtmp);
                            //ESP_LOGI(TAG, "read pat : %s", pat);
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

esp_err_t sup_cmd_manager_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    // UART setup
    ESP_LOGV(TAG, "Configuring UART");
    uart_config_t config;
    config.baud_rate = 115200;
    config.data_bits = UART_DATA_8_BITS;
    config.parity = UART_PARITY_DISABLE;
    config.stop_bits = UART_STOP_BITS_1;
    config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    config.rx_flow_ctrl_thresh = 0;
    config.use_ref_tick = false;
    if((err = uart_param_config(ARDUINO_UART_NUMBER, &config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring UART");
        return err;
    }

    // Set UART pins
    ESP_LOGV(TAG, "Configuring UART pins");
    if((err = uart_set_pin(ARDUINO_UART_NUMBER, RX_ARDUINO_PIN, 
        TX_ARDUINO_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring UART pins");
        return err;
    }

    // Install the driver
    ESP_LOGV(TAG, "Installing UART driver");
    if((err = uart_driver_install(ARDUINO_UART_NUMBER, SUP_CMD_MANAGER_UART_BUFFER_SIZE, 0, 0, NULL, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing UART driver");
        return err;
    }
/*
    // Set uart pattern detect function to detect the 0x7E which is the start of the packet
    uart_enable_pattern_det_intr(ARDUINO_UART_NUMBER, 0x7E, 1, UART_RX_GAP_TOUT_V, 1, 1);
    // Reset the pattern queue length to record at most 10 pattern positions.
    uart_pattern_queue_reset(ARDUINO_UART_NUMBER, 10);

    // Task to sample sensors
    task_handle = xTaskCreateStaticPinnedToCore(sup_cmd_task, "sup_cmd", SUP_CMD_MANAGER_STACK_SIZE, 
        NULL, SUP_CMD_MANAGER_TASK_PRIORITY, stack, &task, SUP_CMD_MANAGER_AFFINITY);*/

    ESP_LOGI(TAG, "Ready");
    return ESP_OK;
}
