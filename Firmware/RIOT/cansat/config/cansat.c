/*
 * config.c
 *
 *  Created on: Nov 12, 2018
 *      Author: tmax4
 */
#include <stdint.h>
#include "cansat.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "stmclk.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/i2c.h"

// GPS UART driver callback
static void gps_uart_rx_interrupt(void *arg, uint8_t data)
{
	(void)arg;
	(void)data;
}

// Command UART driver callback
static void command_uart_rx_interrupt(void *arg, uint8_t data)
{
	(void)arg;
	(void)data;
}

/**
 * This function initializes all the GPIO. Peripheral specific pins are initialized on the respective
 *  peripheral function
 */
static void init_gpio(void)
{
	// Array with the following structure: PORT, PIN, MODE
	unsigned int port_pin_mode[] = {
		ENABLE_5V_PORT, ENABLE_5V_PIN, ENABLE_5V_MODE,
		ENABLE_3V3_PORT, ENABLE_3V3_PIN, ENABLE_3V3_MODE,
		CHG_ALLOWED_PORT, CHG_ALLOWED_PIN, CHG_ALLOWED_MODE,
		CHG_DETECT_PORT, CHG_DETECT_PIN, CHG_DETECT_MODE,
		SW_OPEN_PORT, SW_OPEN_PIN, SW_OPEN_MODE,
		GPOUT_PORT, GPOUT_PIN, GPOUT_MODE,
		P1_PORT, P1_PIN, P1_MODE,
		CHARGING_PORT, CHARGING_PIN, CHARGING_MODE,
		ISET_PORT, ISET_PIN, ISET_MODE,
		IO0_PORT, IO0_PIN, IO0_MODE,
		IO1_PORT, IO1_PIN, IO1_MODE,
		IO2_PORT, IO2_PIN, IO2_MODE,
		IO3_PORT, IO3_PIN, IO3_MODE,
		CHG_ENABLE_PORT, CHG_ENABLE_PIN, CHG_ENABLE_MODE,
		S_GLOBO_PORT, S_GLOBO_PIN, S_GLOBO_MODE,
		S_PARACAIDAS_PORT, S_PARACAIDAS_PIN, S_PARACAIDAS_MODE,
		I2C_ENABLE_PORT, I2C_ENABLE_PIN, I2C_ENABLE_MODE,
		SPI_BUFFER_ENABLE_PORT, SPI_BUFFER_ENABLE_PIN, SPI_BUFFER_ENABLE_MODE,
		IO_ENABLE_PORT, IO_ENABLE_PIN, IO_ENABLE_MODE};

	// Initialize each pin
	for (unsigned int n = 0; n < (sizeof(port_pin_mode) / sizeof(int)) - 3; n += 3)
	{
		gpio_init(GPIO_PIN(port_pin_mode[n], port_pin_mode[n + 1]), port_pin_mode[n + 2]);
	}
}

void cansat_init(void)
{
	// Configure the system clock. The definitions to set the core clock are in
	// 	RIOT/boards/nucleo-f303re/periph_conf.h
	// THIS DOES NOT CONFIGURE THE USB CLOCK.
	// See: https://riot-os.org/api/group__cpu__stm32__common.html
	stmclk_init_sysclk();

	// Configure GPIO pins
	init_gpio();

	// Init ADC1 and ADC2
	MX_ADC1_Init();
	MX_ADC2_Init();

	// Init UART
	uart_init(CONSOLE_UART, 115200, NULL, NULL);
	uart_init(GPS_UART, 9600, gps_uart_rx_interrupt, NULL);
	uart_init(COMMAND_UART, 115200, command_uart_rx_interrupt, NULL);
}
