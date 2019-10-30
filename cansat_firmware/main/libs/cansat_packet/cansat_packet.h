#ifndef __CANSAT_PACKET_H__
#define __CANSAT_PACKET_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    CANSAT_ERRORS = 0x00,
    CANSAT_PARACHUTE_STATE = 0x01,
    CANSAT_PARACHUTE = 0x02,
    CANSAT_BALLOON_STATE = 0x03,
    CANSAT_BALLOON = 0x04,
    CANSAT_READ_SENSOR = 0x05,
    CANSAT_GET_BATTERY = 0x06,
    CANSAT_SET_REPORT_FREQUENCY = 0x07,
    CANSAT_ENABLE_DISABLE_REPORT = 0x08,
    CANSAT_ENABLE_DISABLE_PS = 0x09,
    CANSAT_I2C_BUFFER = 0x0A,
    CANSAT_SPI_BUFFER = 0x0B,
    CANSAT_SEND_DATA = 0x0C,
    CANSAT_UNKNOWN
} cansat_packet_type_t;

typedef enum
{
    BATTERY_VOLTAGE = 0x01,
    BATTERY_CURRENT = 0x02,
    V3V3_VOLTAGE = 0x03,
    V3V3_CURRENT = 0x04,
    V5V_VOLTAGE = 0x05,
    V5V_CURRENT = 0x06,
    POSITION = 0x07,
    GYROSCOPE = 0x08,
    MAGNETOMETER = 0x09,
    ACCELEROMETER = 0x0A,
    ORIENTATION = 0x0B,
    TEMPERATURE = 0x0C,
    HUMIDITY = 0x0D,
    PRESSURE = 0x0E,
    ALTITUDE = 0x0F,
    UNKNOWN_SENSOR
} cansat_sensor_type_t;

typedef enum 
{
    INVALID_COMMAND = 1,
    WRONG_COMMAND_LENGTH = 2,
    WRONG_CHECKSUM = 3,
    BATTERY_OVERCURRENT = 10,
    OVERVOLTAGE_3V3 = 11,
    OVERCURRENT_3V3 = 12,
    OVERVOLTAGE_5V = 13,
    OVERCURRENT_5V = 14
} cansat_error_t;

bool cansat_packet_init(void);

/**
 * @brief Get packet type from the data bytes
 * 
 * @param data bytes containing the entire packet
 * @param length length of the packet
 * @return cansat_packet_type_t type of packet
 */
cansat_packet_type_t cansat_packet_get_type(uint8_t* data, unsigned int length);

bool cansat_packet_decode_report_frequency(uint8_t* data, uint8_t* report_period, unsigned int length);

bool cansat_packet_decode_enable_disable_report(uint8_t* data, bool* enabled, unsigned int length);

/**
 * @brief Decode the sensor IDs from the packet
 * 
 * @param data array of bytes from the packet data to decode
 * @param sensor_id pointer to a cansat_sensor_type_t array to store all the decoded IDs
 * @param decoded_length number of IDs decoded
 * @param length size of the data array
 * @param max_array_size size of the sensor_id array
 * @return true if decoding was successful
 * @return false failed to decode
 */
bool cansat_packet_decode_read_sensors(uint8_t* data, cansat_sensor_type_t* sensor_id, unsigned int* decoded_length, unsigned int length, unsigned int max_array_size);

bool cansat_packet_decode_parachute(uint8_t* data, bool* open, unsigned int length);
bool cansat_packet_decode_balloon(uint8_t* data, bool* open, unsigned int length);

bool cansat_packet_decode_enable_disable_ps(uint8_t* data, bool* enabled, unsigned int length);

#endif // __CANSAT_PACKET_H__
