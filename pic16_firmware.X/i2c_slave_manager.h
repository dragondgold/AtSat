#ifndef I2C_MANAGER_H
#define	I2C_MANAGER_H

#include <xc.h>
#include <stdbool.h>

// Our own address
#define I2C_SLAVE_MANAGER_ADDRESS                   0x02

// Commands
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3_MSB       0x01
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3_LSB       0x02
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_5V_MSB        0x03
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_5V_LSB        0x04
#define I2C_SLAVE_MANAGER_GET_CURRENT_3V3_MSB       0x05
#define I2C_SLAVE_MANAGER_GET_CURRENT_3V3_LSB       0x06
#define I2C_SLAVE_MANAGER_GET_CURRENT_5V_MSB        0x07
#define I2C_SLAVE_MANAGER_GET_CURRENT_5V_LSB        0x08
#define I2C_SLAVE_MANAGER_GET_CURRENT_BAT_MSB       0x09
#define I2C_SLAVE_MANAGER_GET_CURRENT_BAT_LSB       0x0A

// State machine
#define I2C_SLAVE_MANAGER_NO_STATE                  0
#define I2C_SLAVE_MANAGER_SEND_DATA                 3

void i2c_slave_manager_init(void);
void i2c_slave_manager_tasks(void);

#endif