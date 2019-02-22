#include "protocol_decoder.h"

typedef enum
{
    WAITING_START,
    WAITING_LENGTH,
    WAITING_DATA,
} state_machine_state_t;

static state_machine_state_t state;
static void (*packet_decoded)(protocol_packet_t*) = NULL;
static protocol_packet_t packet;
static bool escaping_byte = false;
static uint8_t data_index = 0;

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
        data_index = 0;
        state = WAITING_LENGTH;
        return;
    }
    
    switch(state)
    {
        case WAITING_START:
            break;
            
        case WAITING_LENGTH:
            packet.length = data;
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
                    packet.data[data_index++] = data;
                    
                    // Reached packet length?
                    if(data_index == packet.length)
                    {
                        state = WAITING_START;
                        packet_decoded(&packet);
                    }
                }
            }
            else
            {
                escaping_byte = false;
                packet.data[data_index++] = data ^ 0x20;
                
                // Reached packet length?
                if(data_index == packet.length)
                {
                    state = WAITING_START;
                    packet_decoded(&packet);
                }
            }
            break;
    }
}
