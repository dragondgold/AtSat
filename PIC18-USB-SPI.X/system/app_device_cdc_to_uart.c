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
    LATCbits.LATC4 = !LATCbits.LATC4;
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
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to. So return. */
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
    }
    
    /*
	if (RS232_Out_Data_Rdy == 0)  // only check for new USB buffer if the old RS232 buffer is
	{						  // empty.  This will cause additional USB packets to be NAK'd
		LastRS232Out = getsUSBUSART(RS232_Out_Data,64); //until the buffer is free.
		if(LastRS232Out > 0)
		{
			RS232_Out_Data_Rdy = 1;  // signal buffer full
			RS232cp = 0;  // Reset the current position
		}
	}

    //Check if one or more bytes are waiting in the physical UART transmit
    //queue.  If so, send it out the UART TX pin.
	if(RS232_Out_Data_Rdy && mTxRdyUSART())
	{
    	#if defined(USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL)
        	//Make sure the receiving UART device is ready to receive data before
        	//actually sending it.
        	if(UART_CTS == USB_CDC_CTS_ACTIVE_LEVEL)
        	{
        		USART_putcUSART(RS232_Out_Data[RS232cp]);
        		++RS232cp;
        		if (RS232cp == LastRS232Out)
        			RS232_Out_Data_Rdy = 0;
    	    }
	    #else
	        //Hardware flow control not being used.  Just send the data.
    		USART_putcUSART(RS232_Out_Data[RS232cp]);
    		++RS232cp;
    		if (RS232cp == LastRS232Out)
    			RS232_Out_Data_Rdy = 0;
	    #endif
	}

    //Check if we received a character over the physical UART, and we need
    //to buffer it up for eventual transmission to the USB host.
	if(mDataRdyUSART() && (NextUSBOut < (CDC_DATA_OUT_EP_SIZE - 1)))
        {
		USB_Out_Buffer[NextUSBOut] = USART_getcUSART();
		++NextUSBOut;
		USB_Out_Buffer[NextUSBOut] = 0;
	}

	#if defined(USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL)
    	//Drive RTS pin, to let UART device attached know if it is allowed to
    	//send more data or not.  If the receive buffer is almost full, we
    	//deassert RTS.
    	if(NextUSBOut <= (CDC_DATA_OUT_EP_SIZE - 5u))
    	{
            UART_RTS = USB_CDC_RTS_ACTIVE_LEVEL;
        }
        else
        {
        	UART_RTS = (USB_CDC_RTS_ACTIVE_LEVEL ^ 1);
        }
    #endif

    //Check if any bytes are waiting in the queue to send to the USB host.
    //If any bytes are waiting, and the endpoint is available, prepare to
    //send the USB packet to the host.
	if((USBUSARTIsTxTrfReady()) && (NextUSBOut > 0))
	{
		putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
		NextUSBOut = 0;
	}*/

    CDCTxService();
}