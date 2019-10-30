#include "system.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../mcc_generated_files/mcc.h"
#include "usb.h"
#include "app_device_cdc_to_uart.h"
#include "usb_config.h"
#include "../protocol_decoder.h"

typedef enum
{
    SM_IDLE,
    SM_START_DATA_SEND,
    SM_DATA_SEND
} tx_state_machine_state_t;

static uint8_t usb_out_buffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t usb_in_buffer[CDC_DATA_IN_EP_SIZE];
static uint8_t transferred_bytes = 0;

static uint8_t temp_buffer[64];
static uint8_t tx_data_size = 0;
static tx_state_machine_state_t tx_state = SM_IDLE;
static uint8_t data_index = 0;
static uint8_t usb_buffer_index = 0;
static uint8_t checksum = 0;
static bool checksum_sent = false;

static void check_and_add_checksum(void)
{
    // Calculate the checksum
    uint8_t c = 0xFF - checksum;

    // Need to escape the checksum?
    if(c == PROTOCOL_DECODER_START_BYTE || c == PROTOCOL_DECODER_ESCAPE_BYTE)
    {
        // Is there room for the 2 bytes?
        if(usb_buffer_index < (CDC_DATA_OUT_EP_SIZE - 2))
        {
            usb_out_buffer[usb_buffer_index++] = PROTOCOL_DECODER_ESCAPE_BYTE;
            usb_out_buffer[usb_buffer_index++] = c ^ 0x20;
            checksum_sent = true;
        }
    }
    else
    {
        usb_out_buffer[usb_buffer_index++] = 0xFF - checksum;
        checksum_sent = true;
    }
}

static void packet_decoded(protocol_packet_t *packet)
{
    // Change LED state every time a packet is received
    LED_Toggle();
    
    if(packet->valid && packet->length < sizeof(temp_buffer))
    {
        switch(packet->packet_type)
        {
            case DATA_SEND:
                // Copy the bytes to the temp buffer
                memcpy(temp_buffer, packet->data, packet->length);
                
                // Send the bytes through SPI and get the response
                tx_data_size = packet->length;
                uint8_t *data = temp_buffer;
                
                CS_SetLow();
                
                uint8_t b = 0;
                while(tx_data_size--)
                {
                    __delay_us(1);
                    b = *data;
                    *data = SPI1_Exchange8bit(b);
                    data++;
                }
                
                CS_SetHigh();
                
                // Mark the data as needed to be sent
                tx_data_size = packet->length;
                tx_state = SM_START_DATA_SEND;
                break;
        }
    }
}

void APP_DeviceCDCEmulatorInitialize()
{
    CDCInitEP();

    // Serial port settings
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 115200;

    //Initialize the array
    for (unsigned int i = 0; i < sizeof(usb_out_buffer); i++)
    {
        usb_out_buffer[i] = 0;
    }

    transferred_bytes = 0;
    tx_data_size = 0;
    tx_state = SM_IDLE;
    protocol_decoder_init(packet_decoded);
}

/*********************************************************************
* Function: void APP_DeviceCDCEmulatorTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCDCEmulatorInitialize() and APP_DeviceCDCEmulatorStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCEmulatorTasks()
{   
    if(USBGetDeviceState() < CONFIGURED_STATE)
    {
        return;
    }
    
    if(USBIsDeviceSuspended() == true)
    {
        return;
    }

    // Don't read data when there's a pending response
    if(tx_state == SM_IDLE)
    {
        // Read data from the USB
        transferred_bytes = getsUSBUSART(usb_in_buffer, CDC_DATA_IN_EP_SIZE);

        // Any data available?
        if(transferred_bytes > 0)
        {   
            // Add all the bytes to the decoder
            for(uint8_t n = 0; n < transferred_bytes; ++n)
            {
                protocol_decoder_add_data(usb_in_buffer[n]);
            }

            //putUSBUSART(usb_in_buffer, transferred_bytes);
        }
    }
    
    // Can we send data?
    if(USBUSARTIsTxTrfReady())
    {
        switch(tx_state)
        {
            case SM_IDLE:
                break;
                
            case SM_START_DATA_SEND:
                // Now send the resulting bytes. Encode them and send as possible
                //  so we don't need to have a another big buffer to store the
                //  entire packet, we don't have that much RAM.
                data_index = 0;
                usb_buffer_index = 0;

                // Set the start byte and length bytes of the packet
                usb_out_buffer[usb_buffer_index++] = PROTOCOL_DECODER_START_BYTE;
                usb_out_buffer[usb_buffer_index++] = tx_data_size + 1;
                if(usb_out_buffer[usb_buffer_index-1] == PROTOCOL_DECODER_ESCAPE_BYTE)
                {
                    usb_out_buffer[usb_buffer_index-1] = PROTOCOL_DECODER_ESCAPE_BYTE;
                    usb_out_buffer[usb_buffer_index] = (tx_data_size + 1) ^ 0x20;
                }
                // Packet type
                usb_out_buffer[usb_buffer_index++] = DATA_SEND;
                tx_state = SM_DATA_SEND;
                
                // Initial checksum value
                checksum = DATA_SEND;
                checksum_sent = false;
                break;
                
            case SM_DATA_SEND:
                // All data was sent so put the state machine in IDLE
                if(data_index == tx_data_size)
                {
                    // Send the checksum if it wasn't sent yet
                    if(!checksum_sent)
                    {
                        usb_buffer_index = 0;
                        check_and_add_checksum();
                        putUSBUSART(usb_out_buffer, usb_buffer_index);
                        checksum_sent = true;
                    }
                    else
                    {
                        tx_state = SM_IDLE;
                    }
                }
                else
                {
                    // Send the data
                    for(;usb_buffer_index < CDC_DATA_OUT_EP_SIZE && data_index < tx_data_size; ++usb_buffer_index, ++data_index)
                    {
                        usb_out_buffer[usb_buffer_index] = temp_buffer[data_index];
                        checksum += temp_buffer[data_index];
                    }

                    // If the entire packet was pushed and there's still place
                    //  for the checksum byte, add it now
                    if(data_index == tx_data_size && usb_buffer_index < (CDC_DATA_OUT_EP_SIZE-1))
                    {
                        check_and_add_checksum();
                    }
                    
                    // When we reach here, the entire USB buffer was filled or the entire
                    //  packet was pushed into the buffer
                    putUSBUSART(usb_out_buffer, usb_buffer_index);
                }
                break;
        }
    }
    
    CDCTxService();
}