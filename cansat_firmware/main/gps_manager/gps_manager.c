#include "gps_manager.h"

#include "config/cansat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/uart.h"

static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

void gps_manager_init(void)
{
    // Create the mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Configuración del módulo UART
    uart_config_t config;
    config.baud_rate = 9600;
    config.data_bits = UART_DATA_8_BITS;
    config.parity = UART_PARITY_DISABLE;
    config.stop_bits = UART_STOP_BITS_1;
    config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    config.rx_flow_ctrl_thresh = 0;
    config.use_ref_tick = false;
    uart_param_config(GPS_UART_NUMBER, &config);

    // Set UART pins
    uart_set_pin(GPS_UART_NUMBER, UART_PIN_NO_CHANGE, GPS_TX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Install the driver
    uart_driver_install(GPS_UART_NUMBER, 256, 0, 8, NULL, 0);
}