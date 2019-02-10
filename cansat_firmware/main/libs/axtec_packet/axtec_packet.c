#include "axtec_packet.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"

static const char* TAG = "axtec_packet";

static uint8_t compute_checksum(uint8_t* data, unsigned int length)
{
    uint8_t sum = 0;
    for(unsigned int n = 0; n < length; ++n)
    {
        sum += data[n];
    }

    return sum;
}

bool axtec_packet_init(void)
{
    return true;
}

axtec_packet_error_t axtec_packet_decode(axtec_decoded_packet_t* packet, uint8_t* data, unsigned int length)
{
    // We need at least 4 bytes (1 start + 2 length + 1 checksum)
    if(length < 4)
    {
        return LENGTH_ERROR;
    }

    unsigned int index = 0;

    // Find the start byte
    for(;index < length; ++index)
    {
        if(data[index] == AXTEC_PACKET_START_BYTE)
        {
            // Start byte found!
            break;
        }
    }

    // Couldn't find the start byte
    if(index == length)
    {
        return MALFORMED_ERROR;
    }
    ++index;

    // The next two bytes are the packet length, MSB first. Check if they are escaped.
    uint16_t msb_l, lsb_l;
    // MSB length byte
    if(data[index] == AXTEC_PACKET_ESCAPE_BYTE)
    {
        // The next byte is the escaped MSB length byte
        msb_l = data[index + 1] ^ 0x20;
        index += 2;
    }
    else
    {
        msb_l = data[index++];
    }
    // LSB length byte
    if(data[index] == AXTEC_PACKET_ESCAPE_BYTE)
    {
        // The next byte is the escaped LSB length byte
        lsb_l = data[index + 1] ^ 0x20;
        index += 2;
    }
    else
    {
        lsb_l = data[index++];
    }
    // Make the length
    packet->length = (msb_l << 8) | lsb_l;

    // Do we still have bytes?
    if(index >= length)
    {
        // Fail, we still needed bytes for the checksum at the very least
        return LENGTH_ERROR;
    }

    // Data available?
    if(length > 0 && length <= AXTEC_PACKET_MAX_DATA_LENGTH && (index + packet->length) <= length) 
    {
        bool escaping = false;
        for(unsigned int n = 0; n < packet->length; ++index)
        {
            if(escaping)
            {
                packet->data[n++] = data[index] ^ 0x20;
                escaping = false;
            }
            else
            {
                // Escape byte? Read the next byte
                if(data[index] == AXTEC_PACKET_ESCAPE_BYTE)
                {
                    escaping = true;
                }
                else
                {
                    packet->data[n++] = data[index];
                }
            }
        }
    }

    // Compute checksum
    if(index >= length)
    {
        return LENGTH_ERROR;
    }
    else
    {
        // Compute the checksum with the data bytes
        uint8_t checksum = compute_checksum(packet->data, packet->length);

        // Valid checksum?
        if(checksum == 0xFF)
        {
            packet->valid = true;
        }
        else
        {
            packet->valid = false;
        }
        
        return PACKET_OK;
    }
}

bool axtec_packet_encode(axtec_encoded_packet_t* packet, uint8_t* data, unsigned int length)
{
    // Worst case, 1 start byte and 4 bytes for length when both are escaped
    if((length + 5) > AXTEC_PACKET_MAX_DATA_LENGTH)
    {
        return false;
    }
    unsigned int index = 0;

    // Add start byte
    packet->raw[index++] = AXTEC_PACKET_START_BYTE;

    // Add length bytes, MSB first
    uint8_t msb_l = (uint8_t)(length >> 8);
    uint8_t lsb_l = (uint8_t)length;
    // Length MSB byte
    if(msb_l == AXTEC_PACKET_START_BYTE || msb_l == AXTEC_PACKET_ESCAPE_BYTE)
    {
        packet->raw[index++] = AXTEC_PACKET_ESCAPE_BYTE;
        packet->raw[index++] = msb_l ^ 0x20;
    }
    else
    {
        packet->raw[index++] = msb_l;
    }
    // Length LSB byte
    if(lsb_l == AXTEC_PACKET_START_BYTE || lsb_l == AXTEC_PACKET_ESCAPE_BYTE)
    {
        packet->raw[index++] = AXTEC_PACKET_ESCAPE_BYTE;
        packet->raw[index++] = lsb_l ^ 0x20;
    }
    else
    {
        packet->raw[index++] = lsb_l;
    }

    // Add all the bytes and escape them if needed. Compute the checksum too.
    uint8_t checksum = 0;
    for(unsigned int n = 0; n < length; ++n)
    {
        // Check length
        if(index > AXTEC_PACKET_MAX_DATA_LENGTH)
        {
            // Exceeded max packet length
            return false;
        }

        // Need escaping?
        if(data[n] == AXTEC_PACKET_START_BYTE || data[n] == AXTEC_PACKET_ESCAPE_BYTE)
        {
            if(index + 1 > AXTEC_PACKET_MAX_DATA_LENGTH)
            {
                // Exceeded max packet length
                return false;
            }
            checksum += data[n];
            packet->raw[index++] = AXTEC_PACKET_ESCAPE_BYTE;
            packet->raw[index++] = data[n] ^ 0x20;
        }
        else
        {
            checksum += data[n];
            packet->raw[index++] = data[n];
        }
    }

    // Add the checksum
    if(index > AXTEC_PACKET_MAX_DATA_LENGTH)
    {
        // Exceeded max packet length
        return false;
    }
    packet->raw[index] = 0xFF - checksum;

    packet->length = index + 1;
    return true;
}