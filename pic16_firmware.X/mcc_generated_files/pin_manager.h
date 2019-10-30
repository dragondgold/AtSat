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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16LF18325
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15	
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

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IBAT aliases
#define IBAT_TRIS                 TRISAbits.TRISA2
#define IBAT_LAT                  LATAbits.LATA2
#define IBAT_PORT                 PORTAbits.RA2
#define IBAT_WPU                  WPUAbits.WPUA2
#define IBAT_OD                   ODCONAbits.ODCA2
#define IBAT_ANS                  ANSELAbits.ANSA2
#define IBAT_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define IBAT_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define IBAT_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define IBAT_GetValue()           PORTAbits.RA2
#define IBAT_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define IBAT_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define IBAT_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define IBAT_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define IBAT_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define IBAT_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define IBAT_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define IBAT_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set I3V3 aliases
#define I3V3_TRIS                 TRISAbits.TRISA4
#define I3V3_LAT                  LATAbits.LATA4
#define I3V3_PORT                 PORTAbits.RA4
#define I3V3_WPU                  WPUAbits.WPUA4
#define I3V3_OD                   ODCONAbits.ODCA4
#define I3V3_ANS                  ANSELAbits.ANSA4
#define I3V3_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define I3V3_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define I3V3_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define I3V3_GetValue()           PORTAbits.RA4
#define I3V3_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define I3V3_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define I3V3_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define I3V3_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define I3V3_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define I3V3_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define I3V3_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define I3V3_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set I5V aliases
#define I5V_TRIS                 TRISAbits.TRISA5
#define I5V_LAT                  LATAbits.LATA5
#define I5V_PORT                 PORTAbits.RA5
#define I5V_WPU                  WPUAbits.WPUA5
#define I5V_OD                   ODCONAbits.ODCA5
#define I5V_ANS                  ANSELAbits.ANSA5
#define I5V_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define I5V_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define I5V_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define I5V_GetValue()           PORTAbits.RA5
#define I5V_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define I5V_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define I5V_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define I5V_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define I5V_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define I5V_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define I5V_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define I5V_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set SCL1 aliases
#define SCL1_TRIS                 TRISCbits.TRISC0
#define SCL1_LAT                  LATCbits.LATC0
#define SCL1_PORT                 PORTCbits.RC0
#define SCL1_WPU                  WPUCbits.WPUC0
#define SCL1_OD                   ODCONCbits.ODCC0
#define SCL1_ANS                  ANSELCbits.ANSC0
#define SCL1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define SCL1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define SCL1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define SCL1_GetValue()           PORTCbits.RC0
#define SCL1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define SCL1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define SCL1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define SCL1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define SCL1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define SCL1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define SCL1_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define SCL1_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set SDA1 aliases
#define SDA1_TRIS                 TRISCbits.TRISC1
#define SDA1_LAT                  LATCbits.LATC1
#define SDA1_PORT                 PORTCbits.RC1
#define SDA1_WPU                  WPUCbits.WPUC1
#define SDA1_OD                   ODCONCbits.ODCC1
#define SDA1_ANS                  ANSELCbits.ANSC1
#define SDA1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SDA1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SDA1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SDA1_GetValue()           PORTCbits.RC1
#define SDA1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SDA1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SDA1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SDA1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SDA1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SDA1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SDA1_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define SDA1_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set V3V3 aliases
#define V3V3_TRIS                 TRISCbits.TRISC4
#define V3V3_LAT                  LATCbits.LATC4
#define V3V3_PORT                 PORTCbits.RC4
#define V3V3_WPU                  WPUCbits.WPUC4
#define V3V3_OD                   ODCONCbits.ODCC4
#define V3V3_ANS                  ANSELCbits.ANSC4
#define V3V3_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define V3V3_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define V3V3_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define V3V3_GetValue()           PORTCbits.RC4
#define V3V3_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define V3V3_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define V3V3_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define V3V3_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define V3V3_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define V3V3_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define V3V3_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define V3V3_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set V5V aliases
#define V5V_TRIS                 TRISCbits.TRISC5
#define V5V_LAT                  LATCbits.LATC5
#define V5V_PORT                 PORTCbits.RC5
#define V5V_WPU                  WPUCbits.WPUC5
#define V5V_OD                   ODCONCbits.ODCC5
#define V5V_ANS                  ANSELCbits.ANSC5
#define V5V_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define V5V_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define V5V_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define V5V_GetValue()           PORTCbits.RC5
#define V5V_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define V5V_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define V5V_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define V5V_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define V5V_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define V5V_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define V5V_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define V5V_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

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