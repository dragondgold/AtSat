#ifndef I2C_MANAGER_H
#define	I2C_MANAGER_H

#include <xc.h>
#include <stdbool.h>

// Our own address
#define I2C_SLAVE_MANAGER_ADDRESS                   0x02

// Commands
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3           0x01
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_5V            0x02
#define I2C_SLAVE_MANAGER_GET_CURRENT_3V3           0x03
#define I2C_SLAVE_MANAGER_GET_CURRENT_5V            0x04
#define I2C_SLAVE_MANAGER_GET_VOLTAGE_BAT           0x05

// State machine
#define I2C_SLAVE_MANAGER_NO_STATE                  0
#define I2C_SLAVE_MANAGER_WAITING_COMMAND           1
#define I2C_SLAVE_MANAGER_WAITING_COMMAND_VALUE     2
#define I2C_SLAVE_MANAGER_SEND_DATA                 3

void i2c_slave_manager_init(void);

#endif