#include "gps_manager.h"

#include <stdint.h>
#include <string.h>
#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "libs/minmea/minmea.h"
#include "esp_log.h"

static const char* TAG = "gps";

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

static StackType_t stack[GPS_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;
static uint8_t buffer[256];

static struct minmea_sentence_gga last_gga;

static void gps_rcv_task(void* arg) 
{
    size_t len;

    ESP_LOGI(TAG, "Task started");

    while(true)
    {
        // Update every 800 ms (a little less than the update rate)
        ESP_LOGV(TAG, "Going to sleep");
        vTaskDelay(pdMS_TO_TICKS(800));

        // Get number of bytes in the buffer
        ESP_LOGV(TAG, "Checking buffered data");
        if(uart_get_buffered_data_len(GPS_UART_NUMBER, &len) == ESP_OK)
        {
            // Enough bytes to fill the buffer?
            ESP_LOGV(TAG, "Checking buffer size");
            if(len >= sizeof(buffer)) 
            {
                ESP_LOGV(TAG, "Reading bytes");
                if(uart_read_bytes(GPS_UART_NUMBER, buffer, sizeof(buffer), 100 / portTICK_PERIOD_MS) == sizeof(buffer))
                {
                    char* start = (char *)buffer;
                    char* dollar, *asterisk;
                    do {
                        // Find a complete NMEA string. A NMEA string looks like:
                        //  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
                        // So we look for the characters '$' and '*'
                        dollar = strchr(start, '$');
                        asterisk = strchr(dollar, '*');

                        if(dollar && asterisk)
                        {
                            // Next cycle, start the search in the asterisk
                            start = asterisk;

                            // Are we inside the buffer after the 2 additional characters following the '*'?
                            if(asterisk - (char *)buffer + 2 < sizeof(buffer))
                            {
                                // Terminate the string after the '*' and the two numbers
                                *(asterisk + 3) = '\0';
                                struct minmea_sentence_gga frame;

                                // Use strict parsing so the checksum is also checked
                                switch(minmea_sentence_id(dollar, true))
                                {
                                    case MINMEA_INVALID:
                                        break;

                                    case MINMEA_SENTENCE_GGA:
                                        if(minmea_parse_gga(&frame, dollar)) 
                                        {
                                            // Parse successfull!
                                            last_gga = frame;
                                        }
                                        break;

                                    default:
                                        break;
                                }
                            }
                        }
                    } while(dollar && asterisk);
                }
            }
        }

        ESP_LOGV(TAG, "Free stack: %d", uxTaskGetStackHighWaterMark(task_handle));
    }
}

esp_err_t gps_manager_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    // Create the mutex for this resource
    ESP_LOGV(TAG, "Creating mutex");
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // UART setup
    ESP_LOGV(TAG, "Configuring UART");
    uart_config_t config;
    config.baud_rate = 9600;
    config.data_bits = UART_DATA_8_BITS;
    config.parity = UART_PARITY_DISABLE;
    config.stop_bits = UART_STOP_BITS_1;
    config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    config.rx_flow_ctrl_thresh = 0;
    config.use_ref_tick = false;
    if((err = uart_param_config(GPS_UART_NUMBER, &config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring UART");
        return err;
    }

    // Set UART pins
    ESP_LOGV(TAG, "Configuring UART pins");
    if((err = uart_set_pin(GPS_UART_NUMBER, UART_PIN_NO_CHANGE, 
        GPS_TX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring UART pins");
        return err;
    }

    // Install the driver
    ESP_LOGV(TAG, "Installing UART driver");
    if((err = uart_driver_install(GPS_UART_NUMBER, GPS_MANAGER_UART_BUFFER_SIZE, 0, 8, NULL, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing UART driver");
        return err;
    }

    // Task for GPS NMEA receiving and decoding
    ESP_LOGV(TAG, "Creating task");
    task_handle = xTaskCreateStaticPinnedToCore(gps_rcv_task, "gps", GPS_MANAGER_STACK_SIZE, 
        NULL, GPS_MANAGER_TASK_PRIORITY, stack, &task, GPS_MANAGER_AFFINITY);

    ESP_LOGI(TAG, "Ready!");

    return err;
}

bool gps_manager_is_valid_location(void)
{
    return last_gga.fix_quality == 1;
}


struct minmea_sentence_gga gps_manager_get_gga(void)
{
    return last_gga;
}