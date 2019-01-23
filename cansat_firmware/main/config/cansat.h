#ifndef __CANSAT_H__
#define __CANSAT_H__

#include "driver/gpio.h"
#include "driver/uart.h"

#define GPS_UART_NUMBER             UART_NUM_2
#define ARDUINO_UART_NUMBER         UART_NUM_1
#define DEBUG_UART_NUMBER           UART_NUM_0

#define PIC16_I2C_NUMBER            I2C_NUM_0
#define GENERAL_I2C_NUMBER          I2C_NUM_1

#define CHARGER_DETECTION_PIN       GPIO_NUM_36
#define CHARGING_DETECTION_PIN      GPIO_NUM_39

#define SERVO_PARACHUTE_PIN         GPIO_NUM_0
#define SERVO_GLOBO_PIN             GPIO_NUM_26
#define TX_DEBUG_PIN                GPIO_NUM_1
#define RX_DEBUG_PIN                GPIO_NUM_3
#define LED_PIN                     GPIO_NUM_2
#define AUX_PS_ENABLE_PIN           GPIO_NUM_4
#define CHARGER_ISET_PIN            GPIO_NUM_29
#define SDO_PIN                     GPIO_NUM_12
#define SDI_PIN                     GPIO_NUM_13
#define SCLK_PIN                    GPIO_NUM_14
#define CS_PIN                      GPIO_NUM_15
#define TX_ARDUINO_PIN              GPIO_NUM_16
#define RX_ARDUINO_PIN              GPIO_NUM_17
#define SCL_PIC16_PIN               GPIO_NUM_18
#define SDA_PIC16_PIN               GPIO_NUM_19
#define SDA_PIN                     GPIO_NUM_21
#define SCL_PIN                     GPIO_NUM_22
#define IO_ENABLE_PIN               GPIO_NUM_23
#define I2C_ENABLE_PIN              GPIO_NUM_25
#define SPI_ENABLE_PIN              GPIO_NUM_27
#define GPOUT_PIN                   GPIO_NUM_34
#define GPS_TX_PIN                  GPIO_NUM_35      

#endif