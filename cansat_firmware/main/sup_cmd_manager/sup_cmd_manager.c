#include "sup_cmd_manager.h"
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "config/cansat.h"
#include "libs/axtec_packet/axtec_packet.h"
#include "com_manager/com_manager.h"

#include "esp_log.h"

//#define LOG_LOCAL_LEVEL     ESP_LOG_VERBOSE
static const char* TAG = "sup_cmd";

typedef enum
{
    WAITING_START,
    WAITING_LENGTH_MSB,
    WAITING_LENGTH_LSB
} state_machine_status_t;

// Task
static StackType_t stack[SUP_CMD_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

// UART queue
static QueueHandle_t uart_queue;
static uint8_t buffer[SUP_CMD_MANAGER_UART_BUFFER_SIZE] = {0};

// Packet decode
static uint8_t packet_start_bytes[5] = {0};
static unsigned int packet_start_bytes_length = 0;

/**
 * @brief Get the packet length decoded from incoming bytes.
 * 
 * @param data new byte received
 * @return int -1 if no length is decoded yet, otherwise the decoded packet length
 */
static int get_packet_length(uint8_t data)
{
    static state_machine_status_t status = WAITING_START;
    static bool escaping = false;
    static uint16_t msb = 0, lsb = 0;

    // If start byte is detected, it's always the start of a packet
    if(data == AXTEC_PACKET_START_BYTE)
    {
        status = WAITING_LENGTH_MSB;
        escaping = false;
        msb = lsb = 0;
        packet_start_bytes_length = 0;
        packet_start_bytes[packet_start_bytes_length++] = data;
        return -1;
    }

    switch(status)
    {
        case WAITING_START:
            break;

        case WAITING_LENGTH_MSB:
            if(!escaping)
            {
                // Need to escape?
                if(data == AXTEC_PACKET_ESCAPE_BYTE)
                {
                    escaping = true;
                    packet_start_bytes[packet_start_bytes_length++] = data;
                }
                else 
                {
                    msb = data;
                    packet_start_bytes[packet_start_bytes_length++] = msb;
                    status = WAITING_LENGTH_LSB;
                }
            }
            else
            {
                escaping = false;
                msb = data ^ 0x20;
                packet_start_bytes[packet_start_bytes_length++] = msb;
                status = WAITING_LENGTH_LSB;
            }
            break;

        case WAITING_LENGTH_LSB:
            if(!escaping)
            {
                // Need to escape?
                if(data == AXTEC_PACKET_ESCAPE_BYTE)
                {
                    escaping = true;
                    packet_start_bytes[packet_start_bytes_length++] = data;
                }
                else 
                {
                    lsb = data;
                    packet_start_bytes[packet_start_bytes_length++] = lsb;
                    status = WAITING_START;
                    ESP_LOGV(TAG, "Length: %d", (msb << 8) | lsb);
                    return (msb << 8) | lsb;
                }
            }
            else
            {
                escaping = false;
                lsb = data ^ 0x20;
                packet_start_bytes[packet_start_bytes_length++] = lsb;
                status = WAITING_START;
                ESP_LOGV(TAG, "Length: %d", (msb << 8) | lsb);
                return (msb << 8) | lsb;
            }
            break;
    }

    return -1;
}

static void sup_cmd_task(void* arg)
{
    uart_event_t event;
    axtec_decoded_packet_t axtec_decoded_packet;
    uint8_t packet_data[AXTEC_PACKET_MAX_DATA_LENGTH + sizeof(packet_start_bytes)] = {0};
    int packet_length = -1;
    unsigned int data_index = 0;

    while(true)
    {
        // Wait for an UART event
        if(xQueueReceive(uart_queue, &event, portMAX_DELAY)) 
        {
            switch(event.type) 
            {
                // Data received! We'd better handler data event fast, there would be much more data
                //  events than other types of events. If we take too much time on data event, the
                //  queue might be full
                case UART_DATA:
                    {
                        ESP_LOGV(TAG, "Got data!");

                        // Read all the received bytes
                        uart_read_bytes(ARDUINO_UART_NUMBER, buffer, event.size, portMAX_DELAY);
                        
                        for(unsigned int n = 0; n < event.size; ++n)
                        {
                            // Try to decode the packet length
                            if(packet_length < 0 && (packet_length = get_packet_length(buffer[n])) >= 0)
                            {
                                // We need to receive this many bytes to complete the packet. +1 because
                                //  we need the checksum byte too.
                                packet_length = packet_length + 1;
                                // Copy the start and length bytes
                                memcpy(packet_data, packet_start_bytes, packet_start_bytes_length);
                                // Start putting the data bytes after the start and length bytes
                                data_index = packet_start_bytes_length;

                                ESP_LOGD(TAG, "Got packet with size %d", packet_length - 1);
                            }
                            // Storing packet bytes?
                            else if(packet_length >= 0)
                            {
                                ESP_LOGV(TAG, "Adding byte %d at %d", buffer[n], data_index);
                                packet_data[data_index++] = buffer[n];

                                // All bytes were read? Create the packet and decode it
                                if((data_index - packet_start_bytes_length) == packet_length)
                                {
                                    ESP_LOGD(TAG, "Finished receiving packet with data size %d", packet_length - 1);

                                    // Decode the packet
                                    axtec_packet_error_t err = axtec_packet_decode(&axtec_decoded_packet, packet_data, data_index);

                                    // Start waiting for a new packet after this
                                    packet_length = -1;
                                    
                                    if(err == LENGTH_ERROR)
                                    {
                                        ESP_LOGW(TAG, "Packet length error");
                                    }
                                    else if(err == MALFORMED_ERROR)
                                    {
                                        ESP_LOGW(TAG, "Malformed packet");
                                    }
                                    else if(err == PACKET_OK)
                                    {
                                        ESP_LOGD(TAG, "Packet decoded: %d -> %d, %d, %d, %d, %d, %d", data_index, 
                                            packet_data[0], packet_data[1], packet_data[2], packet_data[3], packet_data[4], packet_data[5]);
                                        if(axtec_decoded_packet.valid)
                                        {
                                            ESP_LOGD(TAG, "Packet valid, executing");
                                            com_manager_add_packet(&axtec_decoded_packet);
                                        }
                                        else
                                        {
                                            ESP_LOGW(TAG, "Wrong packet checksum");
                                        }
                                    }
                                }
                            }
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
    config.baud_rate = 9600;
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
    if((err = uart_driver_install(ARDUINO_UART_NUMBER, SUP_CMD_MANAGER_UART_BUFFER_SIZE, 0, 20, &uart_queue, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing UART driver");
        return err;
    }

    // Configure pin
    gpio_config_t io_config;
    io_config.pin_bit_mask = (1 << IO_ENABLE_PIN);
    io_config.mode = GPIO_MODE_DEF_OUTPUT;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&io_config)) != ESP_OK)
    {
        return err;
    }

    // Enable IO buffer
    gpio_set_level(IO_ENABLE_PIN, 1);

    // Task to sample sensors
    task_handle = xTaskCreateStaticPinnedToCore(sup_cmd_task, "sup_cmd", SUP_CMD_MANAGER_STACK_SIZE, 
        NULL, SUP_CMD_MANAGER_TASK_PRIORITY, stack, &task, SUP_CMD_MANAGER_AFFINITY);

    ESP_LOGI(TAG, "Ready");
    return ESP_OK;
}
