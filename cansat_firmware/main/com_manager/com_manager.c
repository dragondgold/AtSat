#include "com_manager.h"

#include <string.h>
#include "config/cansat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "libs/cc1101_driver/cc1101.h"
#include "libs/axtec_packet/axtec_packet.h"
#include "libs/cansat_packet/cansat_packet.h"
#include "libs/minmea/minmea.h"
#include "power_monitor/power_monitor.h"

#include "servo_manager/servo_manager.h"
#include "battery_manager/battery_manager.h"
#include "sensor_manager/sensor_manager.h"
#include "led_manager/led_manager.h"
#include "aux_ps/aux_ps.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "spi_manager/spi_manager.h"
#include "gps_manager/gps_manager.h"

//#define LOG_LOCAL_LEVEL     ESP_LOG_DEBUG
#include "esp_log.h"

static const char* TAG = "com";

// Task
static StackType_t stack_rx[COM_MANAGER_RX_STACK_SIZE], stack_tx[COM_MANAGER_TX_STACK_SIZE], stack_report[COM_MANAGER_REPORT_STACK_SIZE];
static StaticTask_t task_rx, task_tx, task_report;
static TaskHandle_t task_handle_rx, task_handle_tx, task_handle_report;

// Packets
static cc1101_packet_t cc1101_packet;
static axtec_decoded_packet_t axtec_decoded_packet;
static cansat_errors_t errors;
static cansat_sensor_type_t sensor_types[COM_MANAGER_MAX_SENSOR_IDS];

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
    ESP_LOGD(TAG, "Packet type: %d", type);
    switch(type)
    {
        case CANSAT_GET_ERRORS:
            {
                buffer[0] = CANSAT_GET_ERRORS;
                unsigned int length = 1;

                if(errors.invalid_command)
                {
                    buffer[length++] = 1;
                    errors.invalid_command = false;
                }
                if(errors.wrong_command_length)
                {
                    buffer[length++] = 2;
                    errors.wrong_command_length = false;
                }
                if(errors.wrong_checksum)
                {
                    buffer[length++] = 3;
                    errors.wrong_checksum = false;
                }
                if(power_monitor_is_battery_overcurrent())
                {
                    buffer[length++] = 10;
                }
                if(power_monitor_is_3v3_overvoltage())
                {
                    buffer[length++] = 11;
                }
                if(power_monitor_is_3v3_overcurrent())
                {
                    buffer[length++] = 12;
                }
                if(power_monitor_is_5v_overvoltage())
                {
                    buffer[length++] = 13;
                }
                if(power_monitor_is_5v_overcurrent())
                {
                    buffer[length++] = 14;
                }
                if(!gps_manager_is_valid_location())
                {
                    buffer[length++] = 21;
                }

                ESP_LOGD(TAG, "Sending CANSAT_GET_ERRORS packet");
                axtec_packet_encode(&packet_to_send, buffer, length);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_PARACHUTE_STATE:
            // Get the parachute state and add the packet to the TX queue
            buffer[0] = CANSAT_PARACHUTE_STATE;
            buffer[1] = servo_manager_is_parachute_open() ? 0x01 : 0x00;

            ESP_LOGD(TAG, "Sending CANSAT_PARACHUTE_STATE packet");
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;

        case CANSAT_PARACHUTE:
            {
                // Open parachute and send the state
                buffer[0] = CANSAT_PARACHUTE;
                buffer[1] = 0x00;

                bool open = false;
                if(cansat_packet_decode_parachute(packet->data, &open, packet->length))
                {
                    if(open)
                    {
                        buffer[1] = servo_manager_open_parachute() ? 0x01 : 0x00;
                    }
                    else
                    {
                        buffer[1] = servo_manager_close_parachute() ? 0x01 : 0x00;
                    }
                }

                ESP_LOGD(TAG, "Sending CANSAT_PARACHUTE packet ");
                axtec_packet_encode(&packet_to_send, buffer, 2);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_BALLOON_STATE:
            // Get the balloon state and add the packet to the TX queue
            buffer[0] = CANSAT_BALLOON_STATE;
            buffer[1] = servo_manager_is_ballon_open() ? 0x01 : 0x00;

            ESP_LOGD(TAG, "Sending CANSAT_BALLOON_STATE packet");
            axtec_packet_encode(&packet_to_send, buffer, 2);
            xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            break;

        case CANSAT_BALLOON:
            {
                // Open balloon and send the state
                buffer[0] = CANSAT_BALLOON;
                buffer[1] = 0x00;

                bool open = false;
                if(cansat_packet_decode_balloon(packet->data, &open, packet->length))
                {
                    if(open)
                    {
                        buffer[1] = servo_manager_open_balloon() ? 0x01 : 0x00;
                    }
                    else
                    {
                        buffer[1] = servo_manager_close_balloon() ? 0x01 : 0x00;
                    }
                }

                ESP_LOGD(TAG, "Sending CANSAT_BALLOON packet");
                axtec_packet_encode(&packet_to_send, buffer, 2);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;
        
        case CANSAT_READ_SENSOR:
            {
                // Decode sensor(s) ID(s)
                static sensors_data_t sensors_data;
                unsigned int ids_length = 0;
                if(!cansat_packet_decode_read_sensors(packet->data, sensor_types, &ids_length, packet->length, COM_MANAGER_MAX_SENSOR_IDS))
                {
                    ESP_LOGW(TAG, "Error decoding sensor ID");
                    break;
                }

                // Packet type
                buffer[0] = CANSAT_READ_SENSOR;
                unsigned int buffer_index = 1;
                unsigned int total_length = 1;

                // Go through each decoded ID
                for(unsigned int n = 0; n < ids_length; ++n)
                {
                    cansat_sensor_type_t sensor_type = sensor_types[n];

                    ESP_LOGD(TAG, "Sensor ID: %d", sensor_type);

                    switch(sensor_type)
                    {
                        case GYROSCOPE:
                            // Sensor ID
                            buffer[buffer_index++] = GYROSCOPE;
                            memset(buffer + buffer_index, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.x >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.x);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.y >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.y);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.z >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.gyro.z);
                            }
                            else
                            {
                                buffer_index += 6;
                                ESP_LOGW(TAG, "Failed getting gyro data");
                            }

                            total_length += 7;
                            ESP_LOGD(TAG, "Created GYROSCOPE packet");
                            break;

                        case MAGNETOMETER:
                            // Sensor ID
                            buffer[buffer_index++] = MAGNETOMETER;
                            memset(buffer + buffer_index, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.x >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.x);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.y >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.y);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.z >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.mag.z);
                            }
                            else
                            {
                                buffer_index += 6;
                                ESP_LOGW(TAG, "Failed getting mag data");
                            }

                            total_length += 7;
                            ESP_LOGD(TAG, "Created MAGNETOMETER packet");
                            break;

                        case ACCELEROMETER:
                            // Sensor ID
                            buffer[buffer_index++] = ACCELEROMETER;
                            memset(buffer + buffer_index, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.x >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.x);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.y >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.y);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.z >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.acc.z);
                            }
                            else
                            {
                                buffer_index += 6;
                                ESP_LOGW(TAG, "Failed getting acc data");
                            }

                            total_length += 7;
                            ESP_LOGD(TAG, "Created ACCELEROMETER packet");
                            break;

                        case ORIENTATION:
                            // Sensor ID
                            buffer[buffer_index++] = ORIENTATION;
                            memset(buffer + buffer_index, 0x00, 6);

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                // Put the 3 axis in the X, Y, Z order with MSB first
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.x >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.x);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.y >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.y);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.z >> 8);
                                buffer[buffer_index++] = (uint8_t)((int16_t)sensors_data.orientation.z);
                            }
                            else
                            {
                                buffer_index += 6;
                                ESP_LOGW(TAG, "Failed getting orientation data");
                            }

                            total_length += 7;
                            ESP_LOGD(TAG, "Created ORIENTATION packet");
                            break;

                        case TEMPERATURE:
                            // Sensor ID
                            buffer[buffer_index++] = TEMPERATURE;
                            buffer[buffer_index++] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[buffer_index - 1] = (uint8_t)((int8_t)sensors_data.temperature);
                            }

                            total_length += 2;
                            ESP_LOGD(TAG, "Created TEMPERATURE packet");
                            break;

                        case HUMIDITY:
                            // Sensor ID
                            buffer[buffer_index++] = HUMIDITY;
                            buffer[buffer_index++] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[buffer_index - 1] = sensors_data.humidity;
                            }

                            total_length += 2;
                            ESP_LOGD(TAG, "Created HUMIDITY packet");
                            break;

                        case PRESSURE:
                            // Sensor ID
                            buffer[buffer_index++] = PRESSURE;
                            buffer[buffer_index++] = 0;
                            buffer[buffer_index++] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[buffer_index - 2] = (uint8_t)((uint16_t)sensors_data.pressure >> 8);
                                buffer[buffer_index - 1] = (uint8_t)sensors_data.pressure;
                            }

                            total_length += 3;
                            ESP_LOGD(TAG, "Created PRESSURE packet");
                            break;

                        case ALTITUDE:
                            // Sensor ID
                            buffer[buffer_index++] = ALTITUDE;
                            buffer[buffer_index++] = 0;
                            buffer[buffer_index++] = 0;

                            if(sensor_manager_get_data(&sensors_data))
                            {
                                buffer[buffer_index - 2] = (uint8_t)((uint16_t)sensors_data.altitude >> 8);
                                buffer[buffer_index - 1] = (uint8_t)sensors_data.altitude;
                            }

                            total_length += 3;
                            ESP_LOGD(TAG, "Created ALTITUDE packet");
                            break;

                        case POSITION:
                            {
                                // Fill with latitude and longitude 0
                                buffer[buffer_index++] = POSITION;
                                memset(buffer + buffer_index, 0x00, 9);

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
                                    int32_t lat_i = (int32_t)(lat * (0x7FFFFFFF / 180.0));
                                    int32_t lon_i = (int32_t)(lon * (0x7FFFFFFF / 180.0));

                                    // Add the integers to the buffer, latitude first, MSB first
                                    buffer[buffer_index++] = (uint8_t)(lat_i >> 24);
                                    buffer[buffer_index++] = (uint8_t)(lat_i >> 16);
                                    buffer[buffer_index++] = (uint8_t)(lat_i >> 8);
                                    buffer[buffer_index++] = (uint8_t)lat_i;

                                    buffer[buffer_index++] = (uint8_t)(lon_i >> 24);
                                    buffer[buffer_index++] = (uint8_t)(lon_i >> 16);
                                    buffer[buffer_index++] = (uint8_t)(lon_i >> 8);
                                    buffer[buffer_index++] = (uint8_t)lon_i;
                                }
                                else
                                {
                                    buffer_index += 8;
                                }

                                total_length += 9;
                                ESP_LOGD(TAG, "Created POSITION packet");
                            }
                            break;

                        case BATTERY_VOLTAGE:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = BATTERY_VOLTAGE;

                                uint16_t voltage = power_monitor_get_battery_voltage();

                                buffer[buffer_index++] = (uint8_t)(voltage >> 8);
                                buffer[buffer_index++] = (uint8_t)voltage;

                                total_length += 3;
                                ESP_LOGD(TAG, "Created BATTERY_VOLTAGE packet");
                            }
                            break;

                        case BATTERY_CURRENT:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = BATTERY_CURRENT;

                                uint16_t current = power_monitor_get_battery_current();

                                buffer[buffer_index++] = (uint8_t)(current >> 8);
                                buffer[buffer_index++] = (uint8_t)current;

                                total_length += 3;
                                ESP_LOGD(TAG, "Created BATTERY_CURRENT packet");
                            }
                            break;

                        case V3V3_VOLTAGE:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = V3V3_VOLTAGE;

                                uint16_t voltage = power_monitor_get_3v3_voltage();

                                buffer[buffer_index++] = (uint8_t)(voltage >> 8);
                                buffer[buffer_index++] = (uint8_t)voltage;

                                total_length += 3;
                            }
                            break;

                        case V3V3_CURRENT:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = V3V3_CURRENT;

                                uint16_t current = power_monitor_get_3v3_current();

                                buffer[buffer_index++] = (uint8_t)(current >> 8);
                                buffer[buffer_index++] = (uint8_t)current;

                                total_length += 3;
                            }
                            break;

                        case V5V_VOLTAGE:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = V5V_VOLTAGE;

                                uint16_t voltage = power_monitor_get_5v_voltage();

                                buffer[buffer_index++] = (uint8_t)(voltage >> 8);
                                buffer[buffer_index++] = (uint8_t)voltage;

                                total_length += 3;
                            }
                            break;

                        case V5V_CURRENT:
                            {
                                // Sensor ID
                                buffer[buffer_index++] = V5V_CURRENT;

                                uint16_t current = power_monitor_get_5v_current();

                                buffer[buffer_index++] = (uint8_t)(current >> 8);
                                buffer[buffer_index++] = (uint8_t)current;

                                total_length += 3;
                            }
                            break;

                        case UNKNOWN_SENSOR:
                            errors.invalid_command = true;
                            ESP_LOGW(TAG, "Unknown sensor type %d", sensor_type);
                            break;

                        default:
                            errors.invalid_command = true;
                            ESP_LOGE(TAG, "Shouldn't reach default case with sensor type %d", sensor_type);
                            break;
                    }
                }

                // Send all the data now
                if(axtec_packet_encode(&packet_to_send, buffer, total_length))
                {
                    xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
                }
                else
                {
                    ESP_LOGW(TAG, "Failed encoding packet");
                }
            }
            break;

        case CANSAT_GET_BATTERY:
            // Get battery level
            buffer[0] = CANSAT_GET_BATTERY;
            buffer[1] = battery_manager_get().soc;

            ESP_LOGD(TAG, "Sending CANSAT_GET_BATTERY packet");
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

                ESP_LOGD(TAG, "Sending CANSAT_SET_REPORT_FREQUENCY packet");
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

                ESP_LOGD(TAG, "Sending CANSAT_ENABLE_DISABLE_REPORT packet");
                axtec_packet_encode(&packet_to_send, buffer, 2);
                xQueueSendToBack(tx_queue, &packet_to_send, pdMS_TO_TICKS(50));
            }
            break;

        case CANSAT_ENABLE_DISABLE_PS:
            {
                buffer[0] = CANSAT_ENABLE_DISABLE_REPORT;
                buffer[1] = 0x00;

                bool enabled;
                if(cansat_packet_decode_enable_disable_ps(packet->data, &enabled, packet->length))
                {
                    buffer[1] = 0x01;
                    if(enabled)
                    {
                        aux_ps_enable();
                    }
                    else
                    {
                        aux_ps_disable();
                    }
                }

                ESP_LOGD(TAG, "Sending CANSAT_ENABLE_DISABLE_PS packet");
                axtec_packet_encode(&packet_to_send, buffer, 2);
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
        vTaskDelay(pdMS_TO_TICKS(50));

        if(!xSemaphoreTake(cc1101_mutex, pdMS_TO_TICKS(100)))
        {
            ESP_LOGW(TAG, "rx_task couldn't take mutex");
        }

        uint8_t length = cc1101_bytes_in_rx_fifo();
        ESP_LOGV(TAG, "Bytes: %d", length);

        // Check if data has been received and is ready to be read
        if(length > 0 && cc1101_is_packet_sent_available())
        {
            ESP_LOGV(TAG, "Packet available");

            if(led_manager_is_on())
            {
                led_manager_off();
            }
            else
            {
                led_manager_on();
            }

            // Read the packet
            if(cc1101_read_data(&cc1101_packet))
            {
                axtec_packet_error_t code;

                // Decode the packet
                if((code = axtec_packet_decode(&axtec_decoded_packet, cc1101_packet.data, cc1101_packet.length)) == PACKET_OK)
                {
                    ESP_LOGV(TAG, "Packet decoded");
                    // Check if the packet is valid
                    if(axtec_decoded_packet.valid)
                    {
                        // Process the packet and take the neccesary actions
                        ESP_LOGV(TAG, "Packet valid");
                        process_cansat_packet(&axtec_decoded_packet);
                    }
                    else
                    {
                        ESP_LOGW(TAG, "Wrong packet checksum");
                        errors.wrong_checksum = true;
                    }
                }
                else if(code == LENGTH_ERROR)
                {
                    errors.wrong_command_length = true;
                    ESP_LOGW(TAG, "Wrong packet length");
                }
                else
                {
                    ESP_LOGW(TAG, "Error on decode: %d", code);
                }
            }
            else
            {
                ESP_LOGW(TAG, "Error reading packet");
            }
        }

        // Flush the RX FIFO if needed
        if(cc1101_is_rx_overflow())
        {
            ESP_LOGW(TAG, "RX FLUSH");
            cc1101_set_rx(true);
        }

        xSemaphoreGive(cc1101_mutex);
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
            ESP_LOGV(TAG, "Sending data");
            // Take the mutex to send data
            if(!xSemaphoreTake(cc1101_mutex, pdMS_TO_TICKS(500)))
            {
                ESP_LOGW(TAG, "tx_task couldn't take mutex");
            }

            // Retry at most 5 times to send the packet
            bool data_sent = false;
            for(unsigned int n = 0; n < 5; ++n)
            {
                if(cc1101_send_data(packet.raw, packet.length))
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
            ESP_LOGE(TAG, "Shouldn't reach here in tx_task()");
        }
    }
}

static void report_task(void* arg)
{
    static axtec_decoded_packet_t packet_sensors, packet_power;

    // Emulate a packet requesting all the sensors
    packet_sensors.data[0] = CANSAT_READ_SENSOR;
    packet_sensors.data[1] = POSITION;
    packet_sensors.data[2] = GYROSCOPE;
    packet_sensors.data[3] = MAGNETOMETER;
    packet_sensors.data[4] = ACCELEROMETER;
    packet_sensors.data[5] = TEMPERATURE;
    packet_sensors.data[6] = HUMIDITY;
    packet_sensors.data[7] = PRESSURE;
    packet_sensors.data[8] = ALTITUDE;
    packet_sensors.length = 9;
    packet_sensors.valid = true;

    packet_power.data[0] = CANSAT_READ_SENSOR;
    packet_power.data[1] = BATTERY_VOLTAGE;
    packet_power.data[2] = BATTERY_CURRENT;
    packet_power.data[3] = V3V3_VOLTAGE;
    packet_power.data[4] = V3V3_CURRENT;
    packet_power.data[5] = V5V_VOLTAGE;
    packet_power.data[6] = V5V_CURRENT;
    packet_power.length = 7;
    packet_power.valid = true;

    while(true)
    {
        // Send the data automatically every 'report_frequency'
        vTaskDelay(report_frequency);

        if(reports_enabled)
        {
            // Send all the sensors data in one packet
            process_cansat_packet(&packet_sensors);

            // Send all the voltages and currents now
            process_cansat_packet(&packet_power);
        }
    }
}

esp_err_t com_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");
    
    cc1101_mutex = xSemaphoreCreateMutexStatic(&cc1101_mutex_buffer);

    for(unsigned int n = 0; n < 10; ++n)
    {
        ESP_LOGI(TAG, "Initializing %d/10", n + 1);

        if(!cc1101_init())
        {
            ESP_LOGE(TAG, "Error initializing CC1101 driver");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        else
        {
            // Queue
            tx_queue = xQueueCreateStatic(COM_MANAGER_QUEUE_SIZE, COM_MANAGER_QUEUE_ELEMENT_SIZE, tx_queue_storage,
                                        &tx_static_queue);

            // Create tasks
            ESP_LOGV(TAG, "Creating tasks");
            task_handle_rx = xTaskCreateStaticPinnedToCore(rx_task, "com_rx", COM_MANAGER_RX_STACK_SIZE, 
                NULL, COM_MANAGER_RX_TASK_PRIORITY, stack_rx, &task_rx, COM_MANAGER_RX_AFFINITY);
            task_handle_tx = xTaskCreateStaticPinnedToCore(tx_task, "com_tx", COM_MANAGER_TX_STACK_SIZE, 
                NULL, COM_MANAGER_TX_TASK_PRIORITY, stack_tx, &task_tx, COM_MANAGER_TX_AFFINITY);
            task_handle_report = xTaskCreateStaticPinnedToCore(report_task, "report", COM_MANAGER_REPORT_STACK_SIZE, 
                NULL, COM_MANAGER_REPORT_TASK_PRIORITY, stack_report, &task_report, COM_MANAGER_REPORT_AFFINITY);

            // By default put the transceiver in receive mode
            cc1101_set_rx(true);

            // Packet decoder/encoder
            axtec_packet_init();
            cansat_packet_init();

            ESP_LOGI(TAG, "Ready!");

            return ESP_OK;
        }
    }

    ESP_LOGE(TAG, "Error initializing!");
    return ESP_FAIL;
}