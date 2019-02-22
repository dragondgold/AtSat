#ifndef PROTOCOL_DECODER_H
#define	PROTOCOL_DECODER_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#define PROTOCOL_DECODER_MAX_PACKET_SIZE    64
#define PROTOCOL_DECODER_START_BYTE         0x7E
#define PROTOCOL_DECODER_ESCAPE_BYTE        0x7D

typedef enum
{
    DATA_SEND = 0x01
} packet_type_t;

typedef struct
{
    packet_type_t packet_type;
    uint8_t data[PROTOCOL_DECODER_MAX_PACKET_SIZE];
    uint8_t length;
    bool valid;
} protocol_packet_t;

void protocol_decoder_init(void (*callback)(protocol_packet_t*));
void protocol_decoder_add_data(uint8_t data);

#endif

