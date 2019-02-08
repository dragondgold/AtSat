#include "i2c_slave_manager.h"
#include "mcc_generated_files/mcc.h"
#include <stdbool.h>

static volatile __bit is_read_mode;
static volatile uint8_t state_machine_status;

static volatile uint8_t current_command;
static volatile uint8_t current_command_value;
static volatile uint8_t current_read_address;

static volatile uint8_t weight_calibrate_buffer[4];
static volatile uint8_t weight_calibrate_index;

static void _i2c_slave_manager_i2c1_interrupt(void) {/*
    PIR1bits.SSP1IF = 0;
    
    // Address received!
    if(i2c1_driver_isAddress()) {
        current_read_address = i2c1_driver_getRXData();
        is_read_mode = i2c1_driver_isRead();
        
        // Write mode
        if(!is_read_mode) {
            state_machine_status = I2C_SLAVE_MANAGER_WAITING_COMMAND;
        }
        // Read mode
        else {
            state_machine_status = I2C_SLAVE_MANAGER_SEND_DATA;
        }
    }
    
    // If the buffer is full, clear it by reading it
    if(SSP1STATbits.BF) {
        // Dummy read
        volatile uint8_t d = i2c1_driver_getRXData();
    }
    
    switch(state_machine_status) {
        case I2C_SLAVE_MANAGER_WAITING_COMMAND:
            if(i2c1_driver_isData()) {
                current_command = i2c1_driver_getRXData();
                
                // If we receive another byte, it should be the command value (optional)
                state_machine_status = I2C_SLAVE_MANAGER_WAITING_COMMAND_VALUE;
                
                if(current_command == I2C_SLAVE_MANAGER_CALIBRATE) {
                    weight_calibrate_index = 0;
                }
            }
            break;
            
        case I2C_SLAVE_MANAGER_WAITING_COMMAND_VALUE:
            if(i2c1_driver_isData()) {
                current_command_value = i2c1_driver_getRXData();

                // Set the value for the proper register
                switch(current_command) {
                    case I2C_SLAVE_MANAGER_SAMPLE_START_REGISTER:
                        if(current_command_value) {
                            //ads1231_start_read_weight();
                        }
                        break;
                        
                    case I2C_SLAVE_MANAGER_WEIGHT_REGISTER:
                        break;
                        
                    case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+1:
                        break;
                        
                    case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+2:
                        break;
                        
                    case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+3:
                        break;
                        
                    case I2C_SLAVE_MANAGER_CALIBRATE:
                        // Receive the 4 bytes from the current weight to calibrate the system
                        if(weight_calibrate_index < 4) {
                            // MSB is received first
                            weight_calibrate_buffer[weight_calibrate_index++] = current_command_value;
                            
                            // Received all the bytes? Start calibration
                            if(weight_calibrate_index == 4) {
                                uint32_t weight = 0;
                                weight = ((uint32_t)weight_calibrate_buffer[0] << 24) | ((uint32_t)weight_calibrate_buffer[1] << 16) |
                                        ((uint32_t)weight_calibrate_buffer[2] << 8) | weight_calibrate_buffer[3];
                                
                                //ads1231_start_calibration(weight);
                            }
                        }
                        break;
                }
            }
            
            state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
            break;
            
        case I2C_SLAVE_MANAGER_SEND_DATA:
            // Don't send more data if the master sent an NACK
            if(i2c1_driver_isNACK()) {
                state_machine_status = I2C_SLAVE_MANAGER_NO_STATE;
                break;
            }
            
            switch(current_command) {
                case I2C_SLAVE_MANAGER_STATUS_REGISTER:
                    // Send the weight status
                    //i2c1_driver_TXData(ads1231_weight_ready());
                    break;
                    
                // Start with the LSB
                case I2C_SLAVE_MANAGER_WEIGHT_REGISTER:
                    //i2c1_driver_TXData(ads1231_get_weight());
                    ++current_command;
                    break;

                case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+1:
                    //i2c1_driver_TXData(ads1231_get_weight() >> 8);
                    ++current_command;
                    break;

                case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+2:
                    //i2c1_driver_TXData(ads1231_get_weight() >> 16);
                    ++current_command;
                    break;

                case I2C_SLAVE_MANAGER_WEIGHT_REGISTER+3:
                    //i2c1_driver_TXData(ads1231_get_weight() >> 24);
                    break;
            }
            break;
    }
    
    // Ensure SCL is released
    SSP1CON1bits.CKP = 1;
    
    // Clear if an overrun occurred
    if(SSP1CON1bits.SSPOV) {
        SSP1CON1bits.SSPOV = 0;
    }*/
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
