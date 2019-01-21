#ifndef __I2C_MANAGER_H__
#define __I2C_MANAGER_H__

#include <stdbool.h>

void i2c_manager_init(void);

void i2c_manager_enable_buffer(void);
void i2c_manager_disable_buffer(void);
bool i2c_manager_is_buffer_disabled(void);

#endif