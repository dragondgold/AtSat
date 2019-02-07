#ifndef __CANSAT_PACKET_H__
#define __CANSAT_PACKET_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    CANSAT_GET_ERRORS = 0x00,
    CANSAT_PARACHUTE_STATE = 0x01,
    CANSAT_OPEN_PARACHUTE = 0x02,
    CANSAT_BALLOON_STATE = 0x03,
    CANSAT_OPEN_BALLOON = 0x04,
    CANSAT_READ_SENSOR = 0x05,
    CANSAT_GET_BATTERY = 0x06,
    CANSAT_SET_REPORT_FREQUENCY = 0x07,
    CANSAT_ENABLE_DISABLE_REPORT = 0x08,
    CANSAT_GET_POSITION = 0x09,
    CANSAT_UNKNOWN
} cansat_packet_type_t;

typedef enum
{
    GYROSCOPE = 0x01,
    MAGNETOMETER = 0x02,
    ACCELEROMETER = 0x03,
    ORIENTATION = 0x04,
    TEMPERATURE = 0x05,
    HUMIDITY = 0x06,
    PRESSURE = 0x07,
    ALTITUDE = 0x08,
    BATTERY_VOLTAGE = 0x09,
    BATTERY_CURRENT = 0x0A,
    3V3_VOLTAGE = 0x0B,
    3V3_CURRENT = 0x0C,
    5V_VOLTAGE = 0x0D,
    5V_CURRENT = 0x0E,
    UNKNOWN_SENSOR
} cansat_sensor_type_t;

bool cansat_packet_init(void);

/**
 * @brief Get packet type from the data bytes
 * 
 * @param data bytes containing the entire packet
 * @param length length of the packet
 * @return cansat_packet_type_t type of packet
 */
cansat_packet_type_t cansat_packet_get_type(uint8_t* data, unsigned int length);

bool cansat_packet_decode_read_sensor(uint8_t* data);

bool cansat_packet_decode_report_frequency(uint8_t* data, uint8_t* report_period, unsigned int length);

bool cansat_packet_decode_enable_disable_report(uint8_t* data, bool* enabled, unsigned int length);

bool cansat_packet_decode_read_sensor(uint8_t* data, uint8_t* sensor_id, unsigned int length);

#endif // __CANSAT_PACKET_H__
