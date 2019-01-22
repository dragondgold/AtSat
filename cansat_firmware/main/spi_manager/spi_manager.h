#ifndef __SPI_MANAGER_H__
#define __SPI_MANAGER_H__

#include <stdbool.h>
#include "esp_system.h"

esp_err_t spi_manager_init(void);

void spi_manager_enable_buffer(void);
void spi_manager_disable_buffer(void);
bool spi_manager_is_buffer_disabled(void);

#endif