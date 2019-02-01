#ifndef __SPI_MANAGER_H__
#define __SPI_MANAGER_H__

#include <stdbool.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

esp_err_t spi_manager_init(void);

/**
 * @brief Acquire permission to use the SPI port
 * 
 * @param timeout max time to wait for the port
 * @return esp_err_t ESP_OK if the port was acquired.
 */
esp_err_t spi_manager_acquire(TickType_t timeout);
/**
 * @brief Release the SPI port so others can use it
 * 
 * @return esp_err_t ESP_OK if the port was released.
 */
esp_err_t spi_manager_release(void);

/**
 * @brief Transmit the requested spi_transaction_t to the SPI bus. This waits for
 *  the SPI data to be send synchronously, this method is not async.
 * 
 * @param trans transaction to send and used to receive the data back
 * @param timeout max time to wait for the SPI bus to be available
 * @return esp_err_t ESP_OK if transaction was successfull
 */
esp_err_t spi_manager_device_transmit(spi_transaction_t* trans, TickType_t timeout);

void spi_manager_enable_buffer(void);
void spi_manager_disable_buffer(void);
bool spi_manager_is_buffer_disabled(void);

#endif