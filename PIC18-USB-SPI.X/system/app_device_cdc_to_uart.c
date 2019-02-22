#include "system.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "usb.h"
#include "app_device_cdc_to_uart.h"
#include "usb_config.h"
#include "../protocol_decoder.h"

static uint8_t usb_output_buffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t usb_input_buffer[CDC_DATA_IN_EP_SIZE];
static uint8_t transferred_bytes = 0;

static void packet_decoded(protocol_packet_t *packet)
{
    // Change LED state every time a packet is received
    LATCbits.LATC4 = !LATCbits.LATC4;
    
    // Check if any bytes are waiting in the queue to send to the USB host.
    // If any bytes are waiting, and the endpoint is available, prepare to
    //  send the USB packet to the host.
	if(USBUSARTIsTxTrfReady())
	{
        for(uint8_t n = 0; n < packet->length; ++n)
        {
            usb_output_buffer[n] = packet->data[n];
        }
		putUSBUSART(usb_output_buffer, packet->length);
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
    for (unsigned int i = 0; i < sizeof(usb_output_buffer); i++)
    {
        usb_output_buffer[i] = 0;
    }

    transferred_bytes = 0;
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

    // Read data from the USB
    transferred_bytes = getsUSBUSART(usb_input_buffer, CDC_DATA_IN_EP_SIZE);
    
    // Any data available?
    if(transferred_bytes > 0)
    {   
        // Add all the bytes to the decoder
        for(uint8_t n = 0; n < transferred_bytes; ++n)
        {
            protocol_decoder_add_data(usb_input_buffer[n]);
        }
        
        //putUSBUSART(usb_input_buffer, transferred_bytes);
    }

    CDCTxService();
}