#ifndef __MX25R6435F_SPI_HAL_H__
#define __MX25R6435F_SPI_HAL_H__

#include "nrf_gpio.h"
#include <stdint.h>

bool flash_hal_init(void);

void flash_hal_set_cs(bool status);
void flash_hal_set_wp(bool status);

uint8_t flash_hal_exchange_byte(uint8_t byte);
void flash_hal_write_bytes(uint8_t *data, unsigned int length);
void flash_hal_read_bytes(uint8_t *out, unsigned int length);

#endif