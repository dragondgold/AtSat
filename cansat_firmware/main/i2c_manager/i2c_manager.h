#ifndef __I2C_MANAGER_H__
#define __I2C_MANAGER_H__

#include <stdbool.h>
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"

esp_err_t i2c_manager_init(void);

void i2c_manager_enable_buffer(void);
void i2c_manager_disable_buffer(void);
bool i2c_manager_is_buffer_disabled(void);

esp_err_t i2c_manager_acquire(i2c_port_t port, TickType_t timeout);
esp_err_t i2c_manager_release(i2c_port_t port);

esp_err_t i2c_manager_read_register(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, uint8_t* value);
esp_err_t i2c_manager_read_register_multiple(i2c_port_t port, TickType_t timeout, uint8_t slave_addr, uint8_t reg_addr, unsigned int length, uint8_t* values);

#endif