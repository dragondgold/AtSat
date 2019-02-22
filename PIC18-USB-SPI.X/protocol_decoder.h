#ifndef PROTOCOL_DECODER_H
#define	PROTOCOL_DECODER_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#define PROTOCOL_DECODER_MAX_PACKET_SIZE    64
#define PROTOCOL_DECODER_START_BYTE         0x7E
#define PROTOCOL_DECODER_ESCAPE_BYTE        0x7D

typedef struct
{
    uint8_t data[PROTOCOL_DECODER_MAX_PACKET_SIZE];
    uint8_t length;
} protocol_packet_t;

void protocol_decoder_init(void (*callback)(protocol_packet_t*));
void protocol_decoder_add_data(uint8_t data);

#endif

