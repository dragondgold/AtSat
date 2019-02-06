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

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

// Task
static StackType_t stack[GPS_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;
static uint8_t buffer[512] = {0};

// GPS data
static struct minmea_sentence_gga last_gga;

static void gps_rcv_task(void* arg) 
{
    size_t len;

    ESP_LOGV(TAG, "Task started");

    while(true)
    {
        // Update every 800 ms (a little less than the update rate)
        ESP_LOGV(TAG, "Going to sleep");
        vTaskDelay(pdMS_TO_TICKS(800));

        ESP_LOGV(TAG, "Reading bytes");
        if((len = uart_read_bytes(GPS_UART_NUMBER, buffer, sizeof(buffer), 100 / portTICK_PERIOD_MS)) > 0)
        {
            // Set the end of the string
            buffer[len] = '\0';

            // Every GPS line starts with a '$' character and ends with a \r\n
            // Find a complete NMEA string. They look like:
            //  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
            // So we look for the characters \r\n to find the complete line
            char* start = (char *)buffer;
            char* line_end = NULL;
            do {
                ESP_LOGV(TAG, "Finding $");
                start = strchr(start, '$');

                if(start != NULL)
                {
                    ESP_LOGV(TAG, "Finding \\n");
                    line_end = strchr(start, '\n');
                }
                else
                {
                    line_end = NULL;
                }
                
                // Found the line end?
                if(line_end)
                {
                    // Finish the line here
                    char backup = *(line_end + 1);
                    *(line_end + 1) = '\0';
                    ESP_LOGV(TAG, "NMEA: %s", start);

                    struct minmea_sentence_gga frame;

                    // Use strict parsing so the checksum is also checked
                    switch(minmea_sentence_id(start, false))
                    {
                        case MINMEA_INVALID:
                            ESP_LOGV(TAG, "Invalid NMEA");
                            break;

                        case MINMEA_SENTENCE_GGA:
                            if(minmea_parse_gga(&frame, start)) 
                            {
                                // Parse successfull!
                                ESP_LOGV(TAG, "GGA decoded");
                                ESP_LOGD(TAG, "Fix quality: %d", frame.fix_quality);
                                ESP_LOGD(TAG, "UTC Time: %02d:%02d:%02d:%d", frame.time.hours, frame.time.minutes, frame.time.seconds, frame.time.microseconds);
                                ESP_LOGD(TAG, "Latitude: %.6f", minmea_tocoord(&frame.latitude));
                                ESP_LOGD(TAG, "Longitude: %.6f", minmea_tocoord(&frame.longitude));
                                ESP_LOGD(TAG, "Altitude: %.2f %c", minmea_tofloat(&frame.altitude), frame.altitude_units);

                                // Change in fix quality?
                                if(last_gga.fix_quality != frame.fix_quality)
                                {
                                    ESP_LOGI(TAG, "New fix quality: %d", frame.fix_quality);
                                }

                                if(xSemaphoreTake(mutex, pdMS_TO_TICKS(300)))
                                {
                                    last_gga = frame;
                                    xSemaphoreGive(mutex);
                                }
                            }
                            break;

                        default:
                            ESP_LOGV(TAG, "Default NMEA");
                            break;
                    }

                    // Next cycle, start the search after this line
                    start = line_end;
                    *(line_end + 1) = backup;
                }
                else
                {
                    ESP_LOGV(TAG, "No \\n found");
                }
            } while(line_end);

            ESP_LOGV(TAG, "Finished reading data");
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

bool gps_manager_get_gga(struct minmea_sentence_gga* data)
{
    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(300)))
    {
        // Copy the data
        memcpy((void *)data, (void *)(&last_gga), sizeof(struct minmea_sentence_gga));
        xSemaphoreGive(mutex);

        return true;
    }
    
    return false;
}