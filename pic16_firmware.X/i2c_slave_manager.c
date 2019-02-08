#include "i2c_slave_manager.h"
#include "mcc_generated_files/mcc.h"
#include <stdbool.h>
#include <stdint.h>

static volatile __bit is_read_mode;
static volatile uint8_t state_machine_status;
static volatile uint8_t current_address;

extern uint16_t results[] = {0, 0, 0, 0, 0};

static void _i2c_slave_manager_i2c1_interrupt(void) {
    // Clear flag
    PIR1bits.SSP1IF = 0;

    // Address received!
    if(i2c1_driver_isAddress()) {
        is_read_mode = i2c1_driver_isRead();

        // Write mode
        if(!is_read_mode) {
            // Write mode is not supported!!
            state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
        }
        // Read mode
        else {
            state_machine_status = I2C_SLAVE_MANAGER_SEND_DATA;
            // Always read all the data
            current_address = I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3_MSB;
        }
    }

    // If the buffer is full, clear it by reading it
    if(SSP1STATbits.BF) {
        // Dummy read
        volatile uint8_t d = i2c1_driver_getRXData();
    }

    switch(state_machine_status) {
        case I2C_SLAVE_MANAGER_NO_STATE:
            break;
            
        case I2C_SLAVE_MANAGER_SEND_DATA:
            // Don't send more data if the master sent a NACK
            if(i2c1_driver_isNACK()) {
                state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
                break;
            }

            switch(current_address) {
                case I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3_MSB:
                    i2c1_driver_TXData(results[0] >> 8);
                    ++current_address;
                    break;

                // Start with the LSB
                case I2C_SLAVE_MANAGER_GET_VOLTAGE_3V3_LSB:
                    i2c1_driver_TXData(results[0]);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_VOLTAGE_5V_MSB:
                    i2c1_driver_TXData(results[1] >> 8);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_VOLTAGE_5V_LSB:
                    i2c1_driver_TXData(results[1]);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_3V3_MSB:
                    i2c1_driver_TXData(results[2] >> 8);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_3V3_LSB:
                    i2c1_driver_TXData(results[2]);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_5V_MSB:
                    i2c1_driver_TXData(results[3] >> 8);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_5V_LSB:
                    i2c1_driver_TXData(results[3]);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_BAT_MSB:
                    i2c1_driver_TXData(results[4] >> 8);
                    ++current_address;
                    break;

                case I2C_SLAVE_MANAGER_GET_CURRENT_BAT_LSB:
                    i2c1_driver_TXData(results[4]);
                    state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
                    break;
            }
            break;
    }

    // Ensure SCL is released
    SSP1CON1bits.CKP = 1;

    // Clear if an overrun occurred
    if(SSP1CON1bits.SSPOV) {
        SSP1CON1bits.SSPOV = 0;
    }
}

void i2c_slave_manager_init(void) {
    i2c1_driver_initSlaveHardware();
    
    // Send ACK when being addressed
    SSP1CON2bits.ACKDT = 0;
    // Enable bus collision detection
    SSP1CON3bits.SBCDE = 1;
    
    mssp1_clearIRQ();
    mssp1_enableIRQ();
    
    i2c1_driver_setAddr(I2C_SLAVE_MANAGER_ADDRESS);
    SSP1CON3bits.BOEN = 1; // Enable buffer overwrite
    
    state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
    
    i2c1_driver_setI2cISR(_i2c_slave_manager_i2c1_interrupt);
}
