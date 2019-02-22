#include "protocol_decoder.h"

typedef enum
{
    WAITING_START,
    WAITING_LENGTH,
    WAITING_LENGTH_ESCAPED,        
    WAITING_DATA,
    WAITING_CHECKSUM,
    WAITING_CHECKSUM_ESCAPED
} state_machine_state_t;

static state_machine_state_t state;
static void (*packet_decoded)(protocol_packet_t*) = NULL;
static protocol_packet_t packet;
static bool escaping_byte = false;
static uint8_t data_index = 0;
static uint8_t checksum = 0;

static inline void add_data_byte(uint8_t d)
{       
    // Add every byte to the checksum
    checksum += d;
    
    // First byte is packet type
    if(data_index == 0)
    {
        packet.packet_type = d;
        ++data_index;
    }
    else
    {
        packet.data[data_index-1] = d;
        ++data_index;
    }
    
    // Reached packet length? Read the checksum now
    if(data_index == packet.length)
    {
        state = WAITING_CHECKSUM;
    }
}

void protocol_decoder_init(void (*callback)(protocol_packet_t*))
{
    packet_decoded = callback;
    escaping_byte = false;
    state = WAITING_START;
}

void protocol_decoder_add_data(uint8_t data)
{
    // Start byte? Then nothing to do, wait for more bytes
    if(data == PROTOCOL_DECODER_START_BYTE)
    {
        escaping_byte = false;
        packet.valid = false;
        data_index = 0;
        checksum = 0;
        state = WAITING_LENGTH;
        return;
    }
    
    switch(state)
    {
        case WAITING_START:
            break;
            
        case WAITING_LENGTH:
            // Escape the length?
            if(data == PROTOCOL_DECODER_ESCAPE_BYTE)
            {
                state = WAITING_LENGTH_ESCAPED;
            }
            else
            {
                packet.length = data;
                state = WAITING_DATA;
            }
            break;
            
        case WAITING_LENGTH_ESCAPED:
            packet.length = data ^ 0x20;
            state = WAITING_DATA;
            break;
            
        case WAITING_DATA:
            if(!escaping_byte)
            {
                if(data == PROTOCOL_DECODER_ESCAPE_BYTE)
                {
                    escaping_byte = true;
                }
                else
                {
                    add_data_byte(data);
                }
            }
            else
            {
                escaping_byte = false;
                add_data_byte(data ^ 0x20);
            }
            break;
            
        case WAITING_CHECKSUM:
             // Escape the checksum?
            if(data == PROTOCOL_DECODER_ESCAPE_BYTE)
            {
                state = WAITING_CHECKSUM_ESCAPED;
            }
            else
            {
                // Checksum valid?
                checksum += data;
                if(checksum == 0xFF)
                {
                    packet.length -= 1;
                    packet.valid = true;
                    packet_decoded(&packet);
                }
                else
                {
                    state = WAITING_START;
                }
            }
            break;
            
        case WAITING_CHECKSUM_ESCAPED:
            // Checksum valid?
            checksum += data ^ 0x20;
            if(checksum == 0xFF)
            {
                packet.length -= 1;
                packet.valid = true;
                packet_decoded(&packet);
            }
            else
            {
                state = WAITING_START;
            }
            break;
    }
}
