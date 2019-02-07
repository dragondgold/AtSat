#ifndef __AXTEC_PACKET_H__
#define __AXTEC_PACKET_H__

#include "esp_system.h"
#include <stdint.h>
#include <stdbool.h>

#define AXTEC_PACKET_MAX_DATA_LENGTH    128

// Packet constants
#define AXTEC_PACKET_START_BYTE         0x7E
#define AXTEC_PACKET_ESCAPE_BYTE        0x7D

typedef struct
{
    uint16_t length;                                // Data length
    uint8_t data[AXTEC_PACKET_MAX_DATA_LENGTH];     // Packet data
    bool valid;                                     // Valid checksum?
} axtec_decoded_packet_t;

typedef struct
{
    uint16_t length;                                // Data length
    uint8_t raw[AXTEC_PACKET_MAX_DATA_LENGTH];      // Packet raw bytes
} axtec_encoded_packet_t;

/**
 * @brief Initialize the packet encoder/decoder
 * 
 * @return true init was successfull
 * @return false problem initializing
 */
bool axtec_packet_init(void);

/**
 * @brief Decode a packet
 * 
 * @param packet pointer to a axtec_decoded_packet_t type to store the decoded packet
 * @param data array of bytes to decode
 * @param length length of the array passed
 * @return true bytes were decoded
 * @return false failed to decode bytes
 */
bool axtec_packet_decode(axtec_decoded_packet_t* packet, uint8_t* data, unsigned int length);

/**
 * @brief Encode a packet
 * 
 * @param packet pointer to a axtec_encoded_packet_t type to store the encoded packet
 * @param data array of bytes to encode
 * @param length length of the array passed
 * @return true bytes were encoded
 * @return false failed to encode bytes
 */
bool axtec_packet_encode(axtec_encoded_packet_t* packet, uint8_t* data, unsigned int length);

#endif // __AXTEC_PACKET_H__
