#include "cansat_packet.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"

static const char* TAG = "can_pack";

bool cansat_packet_init(void)
{
    return true;
}

cansat_packet_type_t cansat_packet_get_type(uint8_t* data, unsigned int length)
{
    // We need at least one byte, the first byte is the data type
    if(length > 0)
    {
        // Valid type?
        if(data[0] >= CANSAT_UNKNOWN) 
        {
            return CANSAT_UNKNOWN;
        }
        
        return (cansat_packet_type_t)data[0];
    }

    return CANSAT_UNKNOWN;
}

bool cansat_packet_decode_report_frequency(uint8_t* data, uint8_t* report_period, unsigned int length)
{
    // Check packet type
    if(cansat_packet_get_type(data, length) == CANSAT_SET_REPORT_FREQUENCY && length > 1)
    {
        *report_period = data[1];
        return true;
    }

    return false;
}

bool cansat_packet_decode_enable_disable_report(uint8_t* data, bool* enabled, unsigned int length)
{
    // Check packet type
    if(cansat_packet_get_type(data, length) == CANSAT_ENABLE_DISABLE_REPORT && length > 1)
    {
        *enabled = data[1];
        return true;
    }

    return false;
}

bool cansat_packet_decode_read_sensor(uint8_t* data, cansat_sensor_type_t* sensor_id, unsigned int length)
{
    // Check packet type
    if(cansat_packet_get_type(data, length) == CANSAT_READ_SENSOR && length > 1)
    {
        if(data[1] >= UNKNOWN_SENSOR)
        {
            *sensor_id = UNKNOWN_SENSOR;
        }
        else
        {
            *sensor_id = data[1];
        }
        return true;
    }

    return false;
}


