#include "com_manager.h"

#include <string.h>
#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "libs/cc1101_driver/cc1101.h"
#include "libs/axtec_packet/axtec_packet.h"
#include "libs/cansat_packet/cansat_packet.h"
#include "libs/minmea/minmea.h"

#include "servo_manager/servo_manager.h"
#include "battery_manager/battery_manager.h"
#include "sensor_manager/sensor_manager.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "spi_manager/spi_manager.h"
#include "gps_manager/gps_manager.h"

#include "esp_log.h"

static const char* TAG = "com";

// Task
static StackType_t stack_rx[COM_MANAGER_RX_STACK_SIZE], stack_tx[COM_MANAGER_TX_STACK_SIZE];
static StaticTask_t task_rx, task_tx;
static TaskHandle_t task_handle_rx, task_handle_tx;

// Packets
static cc1101_packet_t cc1101_packet;
static axtec_decoded_packet_t axtec_decoded_packet;

// Queue
static StaticQueue_t tx_static_queue;
static uint8_t tx_queue_storage[COM_MANAGER_QUEUE_SIZE * COM_MANAGER_QUEUE_ELEMENT_SIZE];
static QueueHandle_t tx_queue;

// Configs
static TickType_t report_frequency = pdMS_TO_TICKS(COM_MANAGER_DEFAULT_REPORT_PERIOD);
static bool reports_enabled = COM_MANAGER_DEFAULT_REPORTS_ENABLED;

// Mutex
static StaticSemaphore_t cc1101_mutex_buffer;
static SemaphoreHandle_t cc1101_mutex;

/**
 * @brief Process the axtec_decoded_packet_t packet and take the neccesary actions
 * 
 * @param packet packet to decode
 */
static void process_cansat_packet(axtec_decoded_packet_t* packet)
{
    static struct minmea_sentence_gga gps_data;
    static axtec_encoded_packet_t packet_to_send;
    static uint8_t buffer[64];

    cansat_packet_type_t type = cansat_packet_get_type(packet->data, packet->length);
    switch(type)
    {
        case CANSAT_GET_ERRORS:
            // TODO: implement
            break;

        case CANSAT_PARACHUTE_STATE:
            // Get the parachute state and add the packet to the TX queue
            buffer[0] = CANSAT_PARACHUTE_STATE;
            buffer[1] = servo_manager_is_parachute_open() ? 0x01 : 0x00;
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;

        case CANSAT_OPEN_PARACHUTE:
            // Open parachute and send the state
            buffer[0] = CANSAT_OPEN_PARACHUTE;
            buffer[1] = servo_manager_open_parachute() ? 0x01 : 0x00;
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;

        case CANSAT_BALLOON_STATE:
            // Get the balloon state and add the packet to the TX queue
            buffer[0] = CANSAT_BALLOON_STATE;
            buffer[1] = servo_manager_is_ballon_open() ? 0x01 : 0x00;
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;

        case CANSAT_OPEN_BALLOON:
            // Open balloon and send the state
            buffer[0] = CANSAT_OPEN_BALLOON;
            buffer[1] = servo_manager_open_balloon() ? 0x01 : 0x00;
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;
        
        case CANSAT_READ_SENSOR:
            {
                static sensors_data_t sensors_data;
                cansat_sensor_type_t sensor_type;
                if(cansat_packet_decode_read_sensor(packet->data, &sensor_type, packet->length))
                {
                    // Packet type
                    buffer[0] = CANSAT_READ_SENSOR;

                    switch(sensor_type)
                    {
                        case GYROSCOPE:
                            // Sensor ID
                            buffer[1] = GYROSCOPE;
                            memset(buffer + 2, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.gyro.x >> 8);
                                buffer[3] = (uint8_t)((uint16_t)sensors_data.gyro.x);
                                buffer[4] = (uint8_t)((uint16_t)sensors_data.gyro.y >> 8);
                                buffer[5] = (uint8_t)((uint16_t)sensors_data.gyro.y);
                                buffer[6] = (uint8_t)((uint16_t)sensors_data.gyro.z >> 8);
                                buffer[7] = (uint8_t)((uint16_t)sensors_data.gyro.z);
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 8);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case MAGNETOMETER:
                            // Sensor ID
                            buffer[1] = MAGNETOMETER;
                            memset(buffer + 2, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.mag.x >> 8);
                                buffer[3] = (uint8_t)((uint16_t)sensors_data.mag.x);
                                buffer[4] = (uint8_t)((uint16_t)sensors_data.mag.y >> 8);
                                buffer[5] = (uint8_t)((uint16_t)sensors_data.mag.y);
                                buffer[6] = (uint8_t)((uint16_t)sensors_data.mag.z >> 8);
                                buffer[7] = (uint8_t)((uint16_t)sensors_data.mag.z);
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 8);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case ACCELEROMETER:
                            // Sensor ID
                            buffer[1] = ACCELEROMETER;
                            memset(buffer + 2, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.acc.x >> 8);
                                buffer[3] = (uint8_t)((uint16_t)sensors_data.acc.x);
                                buffer[4] = (uint8_t)((uint16_t)sensors_data.acc.y >> 8);
                                buffer[5] = (uint8_t)((uint16_t)sensors_data.acc.y);
                                buffer[6] = (uint8_t)((uint16_t)sensors_data.acc.z >> 8);
                                buffer[7] = (uint8_t)((uint16_t)sensors_data.acc.z);
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 8);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case ORIENTATION:
                            // Sensor ID
                            buffer[1] = ORIENTATION;
                            memset(buffer + 2, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.orientation.x >> 8);
                                buffer[3] = (uint8_t)((uint16_t)sensors_data.orientation.x);
                                buffer[4] = (uint8_t)((uint16_t)sensors_data.orientation.y >> 8);
                                buffer[5] = (uint8_t)((uint16_t)sensors_data.orientation.y);
                                buffer[6] = (uint8_t)((uint16_t)sensors_data.orientation.z >> 8);
                                buffer[7] = (uint8_t)((uint16_t)sensors_data.orientation.z);
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 8);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case TEMPERATURE:
                            // Sensor ID
                            buffer[1] = TEMPERATURE;
                            buffer[2] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[2] = (uint8_t)((int8_t)sensors_data.temperature);
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 3);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case HUMIDITY:
                            // Sensor ID
                            buffer[1] = HUMIDITY;
                            buffer[2] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[2] = sensors_data.humidity;
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 3);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case PRESSURE:
                            // Sensor ID
                            buffer[1] = PRESSURE;
                            buffer[2] = 0;
                            buffer[3] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.pressure >> 8);
                                buffer[3] = (uint8_t)sensors_data.pressure;
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 4);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case ALTITUDE:
                            // Sensor ID
                            buffer[1] = ALTITUDE;
                            buffer[2] = 0;
                            buffer[3] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[2] = (uint8_t)((uint16_t)sensors_data.altitude >> 8);
                                buffer[3] = (uint8_t)sensors_data.altitude;
                            }

                            axtec_packet_encode(&packet_to_send, buffer, 4);
                            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            break;

                        case BATTERY_VOLTAGE:
                            {
                                // Sensor ID
                                buffer[1] = BATTERY_VOLTAGE;
                                buffer[2] = 0;
                                buffer[3] = 0;

                                battery_data_t battery = battery_manager_get();

                                buffer[2] = (uint8_t)(battery.volts >> 8);
                                buffer[3] = (uint8_t)battery.volts;

                                axtec_packet_encode(&packet_to_send, buffer, 4);
                                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                            }
                            break;

                        case BATTERY_CURRENT:
                            //TODO: implement
                            break;

                        case 3V3_VOLTAGE:
                            //TODO: implement
                            break;

                        case 3V3_CURRENT:
                            //TODO: implement
                            break;

                        case 5V_VOLTAGE:
                            //TODO: implement
                            break;

                        case 5V_CURRENT:
                            //TODO: implement
                            break;

                        case UNKNOWN_SENSOR:
                            ESP_LOGW(TAG, "Unknown sensor type %d", sensor_type);
                            break;

                        default:
                            ESP_LOGE(TAG, "Shouldn't reach default case with sensor type %d", sensor_type);
                            break;
                    }
                }
            }
            break;

        case CANSAT_GET_BATTERY:
            // Get battery level
            buffer[0] = CANSAT_GET_BATTERY;
            buffer[1] = battery_manager_get().soc;
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;
        
        case CANSAT_SET_REPORT_FREQUENCY:
            {
                buffer[0] = CANSAT_SET_REPORT_FREQUENCY;
                buffer[1] = 0x00;
                
                uint8_t period;
                if(cansat_packet_decode_report_frequency(packet->data, &period, packet->length))
                {
                    report_frequency = pdMS_TO_TICKS((TickType_t)period);
                    buffer[1] = 0x01;
                }

                axtec_packet_encode(&packet_to_send, buffer, 2);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_ENABLE_DISABLE_REPORT:
            {
                buffer[0] = CANSAT_ENABLE_DISABLE_REPORT;
                buffer[1] = 0x00;

                bool enabled;
                if(cansat_packet_decode_enable_disable_report(packet->data, &enabled, packet->length))
                {
                    reports_enabled = enabled;
                    buffer[1] = 0x01;
                }

                axtec_packet_encode(&packet_to_send, buffer, 2);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_GET_POSITION:
            {
                // Fill with latitude and longitude 0
                memset(buffer, 0x00, 9);
                buffer[0] = CANSAT_GET_POSITION;

                if(gps_manager_get_gga(&gps_data))
                {
                    // To ensure cross-platform compatibility when sending the latitude and longitude values
                    //  we convert them to integers instead of sending them in floating point format.
                    // The equation to convert the angle to integer is:
                    //      int encodedAngle = (int)(angle * (0x7FFFFFFF / 180.0));
                    // The equation to convert from integer to angle is:
                    //      float angle = (encodedAngle / (0x7FFFFFFF / 180.0));
                    // See: https://stackoverflow.com/questions/7934623/what-is-the-approximate-resolution-of-a-single-precision-floating-point-number-w
                    float lat = minmea_tocoord(&gps_data.latitude);
                    float lon = minmea_tocoord(&gps_data.longitude);
                    uint32_t lat_i = (uint32_t)(lat * (0x7FFFFFFF / 180.0));
                    uint32_t lon_i = (uint32_t)(lon * (0x7FFFFFFF / 180.0));

                    // Add the integers to the buffer, latitude first, MSB first
                    buffer[1] = (uint8_t)(lat_i >> 24);
                    buffer[2] = (uint8_t)(lat_i >> 16);
                    buffer[3] = (uint8_t)(lat_i >> 8);
                    buffer[4] = (uint8_t)lat_i;

                    buffer[5] = (uint8_t)(lon_i >> 24);
                    buffer[6] = (uint8_t)(lon_i >> 16);
                    buffer[7] = (uint8_t)(lon_i >> 8);
                    buffer[8] = (uint8_t)lon_i;
                }

                // Add the packet to the send queue
                axtec_packet_encode(&packet_to_send, buffer, 9);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_UNKNOWN:
            ESP_LOGW(TAG, "Unknown packet type %d", type);
            break;

        default:
            ESP_LOGE(TAG, "Shouldn't reach default case with type %d", type);
            break;
    }
}

static void rx_task(void* arg) 
{
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        if(!xSemaphoreTake(cc1101_mutex, pdMS_TO_TICKS(100)))
        {
            ESP_LOGW(TAG, "rx_task couldn't take mutex");
        }

        // Check if data has been received and is ready to be read
        if(cc1101_bytes_in_rx_fifo() > 0 && cc1101_is_packet_sent_available())
        {
            // Read the packet
            if(cc1101_read_data(&cc1101_packet))
            {
                xSemaphoreGive(cc1101_mutex);
                // Check packet integrity
                if(cc1101_packet.crc_ok)
                {
                    // Decode the packet
                    if(axtec_packet_decode(&axtec_decoded_packet, cc1101_packet.data, cc1101_packet.length))
                    {
                        // Check if the packet is valid
                        if(axtec_decoded_packet.valid)
                        {
                            // Process the packet and take the neccesary actions
                            process_cansat_packet(&axtec_decoded_packet);
                        }
                    }
                }
                else 
                {
                    ESP_LOGW(TAG, "Wrong packet CRC");
                }
            }
        }
    }
}

static void tx_task(void* arg)
{
    // Don't put this into the stack
    static axtec_encoded_packet_t packet;
    
    while(true)
    {
        // Wait forever for an item
        if(xQueueReceive(tx_queue, &packet, portMAX_DELAY))
        {
            // Take the mutex to send data
            if(!xSemaphoreTake(cc1101_mutex, pdMS_TO_TICKS(500)))
            {
                ESP_LOGW(TAG, "tx_task couldn't take mutex");
            }

            // Retry at most 5 times to send the packet
            bool data_sent = false;
            for(unsigned int n = 0; n < 5; ++n)
            {
                if(cc1101_send_data(packet->raw, packet->length))
                {
                    data_sent = true;
                    break;
                }
            }

            if(!data_sent)
            {
                ESP_LOGW(TAG, "Failed sending data");
            }

            // Set RX mode again and release the mutex
            cc1101_set_rx(true);
            xSemaphoreGive(cc1101_mutex);
        }
        else
        {
            ESP_LOGE(TGA, "Shouldn't reach here in tx_task()");
        }
    }
}

esp_err_t com_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");
    
    cc1101_mutex = xSemaphoreCreateMutexStatic(&cc1101_mutex_buffer);

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

    // Create tasks
    ESP_LOGV(TAG, "Creating tasks");
    task_handle_rx = xTaskCreateStaticPinnedToCore(rx_task, "com_rx", COM_MANAGER_RX_STACK_SIZE, 
        NULL, COM_MANAGER_RX_TASK_PRIORITY, stack_rx, &task_rx, COM_MANAGER_RX_AFFINITY);
    task_handle_tx = xTaskCreateStaticPinnedToCore(tx_task, "com_tx", COM_MANAGER_TX_STACK_SIZE, 
        NULL, COM_MANAGER_TX_TASK_PRIORITY, stack_tx, &task_tx, COM_MANAGER_TX_AFFINITY);

    // Queue
    tx_queue = xQueueCreateStatic(COM_MANAGER_QUEUE_SIZE, COM_MANAGER_QUEUE_ELEMENT_SIZE, tx_queue_storage,
                                &tx_static_queue);

    // By default put the transceiver in receive mode
    cc1101_set_rx();

    // Packet decoder/encoder
    axtec_packet_init();
    cansat_packet_init();

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}