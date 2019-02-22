/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F14K50
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB 	          :  MPLAB X 5.10	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SDI1 aliases
#define SDI1_TRIS                 TRISBbits.TRISB4
#define SDI1_LAT                  LATBbits.LATB4
#define SDI1_PORT                 PORTBbits.RB4
#define SDI1_WPU                  WPUBbits.WPUB4
#define SDI1_ANS                  ANSELHbits.ANS10
#define SDI1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SDI1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SDI1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SDI1_GetValue()           PORTBbits.RB4
#define SDI1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SDI1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SDI1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SDI1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SDI1_SetAnalogMode()      do { ANSELHbits.ANS10 = 1; } while(0)
#define SDI1_SetDigitalMode()     do { ANSELHbits.ANS10 = 0; } while(0)

// get/set SCK1 aliases
#define SCK1_TRIS                 TRISBbits.TRISB6
#define SCK1_LAT                  LATBbits.LATB6
#define SCK1_PORT                 PORTBbits.RB6
#define SCK1_WPU                  WPUBbits.WPUB6
#define SCK1_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define SCK1_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define SCK1_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define SCK1_GetValue()           PORTBbits.RB6
#define SCK1_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define SCK1_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define SCK1_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define SCK1_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISCbits.TRISC4
#define LED_LAT                  LATCbits.LATC4
#define LED_PORT                 PORTCbits.RC4
#define LED_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define LED_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define LED_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define LED_GetValue()           PORTCbits.RC4
#define LED_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set SDO1 aliases
#define SDO1_TRIS                 TRISCbits.TRISC7
#define SDO1_LAT                  LATCbits.LATC7
#define SDO1_PORT                 PORTCbits.RC7
#define SDO1_ANS                  ANSELHbits.ANS9
#define SDO1_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define SDO1_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define SDO1_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define SDO1_GetValue()           PORTCbits.RC7
#define SDO1_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define SDO1_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define SDO1_SetAnalogMode()      do { ANSELHbits.ANS9 = 1; } while(0)
#define SDO1_SetDigitalMode()     do { ANSELHbits.ANS9 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/