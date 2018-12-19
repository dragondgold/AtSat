#include "gps_manager.h"
#include "periph/uart.h"
#include "config/cansat.h"
#include "minmea.h"
#include "mbox.h"
#include <stdint.h>

static uint8_t GPS_RCV_BUFFER[GPS_MANAGER_RCV_BUFFER_SIZE];

// GPS UART driver callback
static void gps_uart_rx_interrupt(void *arg, uint8_t data)
{
	(void)arg;
	(void)data;
}

void gps_manager_init(void) 
{
    // Init the UART to receive the NMEA sentences from the GPS
    uart_init(GPS_UART, 9600, gps_uart_rx_interrupt, NULL);
}