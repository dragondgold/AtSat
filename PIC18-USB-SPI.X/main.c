#include "system.h"
#include "app_device_cdc_to_uart.h"
#include "mcc_generated_files/mcc.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"

void main(void)
{
    USBDeviceInit();
    USBDeviceAttach();
    
    // Initialize the device
    SYSTEM_Initialize();
    
    // Enable high priority global interrupts
    INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    INTERRUPT_GlobalInterruptLowEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    while(1)
    {
        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif


        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to.  So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            continue;
        }

        if( USBIsDeviceSuspended()== true )
        {
            continue;
        }

        //Application specific tasks
        APP_DeviceCDCEmulatorTasks();
    }
}
