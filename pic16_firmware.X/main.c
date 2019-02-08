/**
 * This program samples all 5 channels to measure voltages and currents in
 *  the AXTEC CanSat platform. As we are very CPU constrained we have to do
 *  some tradeoffs in order to make this work, maybe a real ADC chip will do
 *  better in the future.
 * 
 * The firmware samples 13 times all the channels. Every channel and conversion
 *  takes about 29us (plus code and interrupt overhead, the XC8 compiler and
 *  PIC16 architecture aren't the most efficient) so the 5 channels are sampled
 *  every 29*5=145us. After sampling this 13 times, 145*13=1885us were spent
 *  sampling and the accumulated results are stored in the results[]. The ESP32
 *  has then to take the accumulated values and divide them by 13, it's fast for
 *  him, very slow for us that's we don't do it here.
 * 
 * After taking the 13 samples the sampling is stopped and the SDA and SCL lines
 *  are taken from high to low to indicate the ESP32 that data is ready (this is
 *  possible because we are the only device in the I2C bus), also the TMR0 is
 *  started to fire an interrupt after 2ms. This is all the time the ESP32 has
 *  to read the data from the PIC16 before it starts a new sampling process
 *  disabling the I2C again.
 * 
 * We have to disable the I2C because the sampling takes 100% of the CPU time so
 *  we can't process the I2C events at the same time.
 */

#include "mcc_generated_files/mcc.h"
#include "i2c_slave_manager.h"
#include "main.h"

static adc_channel_t channels[] = {V3V3,V5V,I3V3,I5V,IBAT};
static uint8_t channel_index = 0;
static uint8_t sample_counter = 0;

// They results are in the same order as in channels_all[] array
uint16_t results[] = {0, 0, 0, 0, 0};
static __bit data_ready = 0;

// The timer interrupt is fired 2 ms after being started
void tmr0_isr(void) 
{
    // Stop the timer
    T0CON0bits.T0EN = 0;
    // Disable I2C
    SSP1CON1bits.SSPEN = 0;
    PIE1bits.SSP1IE = 0;
    SCL1_SetDigitalInput();
    SDA1_SetDigitalInput();
    
    CLRWDT();
    
    // Restart the ADC sampling process. Select the channel.
    ADCON0bits.CHS = channels[channel_index];
    
    // Load the time to wait for 6 us for the sampling time.
    PIR1bits.TMR2IF = 0;
    PR2 = 0x2F;
    T2CONbits.TMR2ON = 1;
    while(!PIR1bits.TMR2IF);
    
    // Start the conversion
    ADCON0bits.ADGO = 1;
}

void adc_isr(void)
{
    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
    CLRWDT();

    // This is the last channel to sample
    if(channel_index >= sizeof(results))
    {
        if(++sample_counter >= SAMPLES_PER_BATCH)
        {
            // Store the result
            results[channel_index++] = ((ADRESH << 8) + ADRESL);
            sample_counter = 0;
            channel_index = 0;
            
            // Start the timer to start the sampling process later
            TMR0H = 0x7C;
            TMR0L = 0x00;
            T0CON0bits.T0EN = 1;
            
            // Clear the results
            results[0] = 0;
            results[1] = 0;
            results[2] = 0;
            results[3] = 0;
            results[4] = 0;
            
            // Mark as data ready
            data_ready = 1;
            return;
        }
        
        // Keep going!
        channel_index = 0;
    }
    
    // Select the next channel. This takes 14 instructions!!! The free XC8 compiler
    //  is as bad as me writing assembler code.
    ADCON0bits.CHS = channels[channel_index];
    
    // Load the time to wait for 6 us for the sampling time. We use the timer
    //  so we can execute other operation while waiting the sampling time.
    PIR1bits.TMR2IF = 0;
    PR2 = 0x2F;
    T2CONbits.TMR2ON = 1;
    
    // Store the result, add all the result. The reader must then do an average.
    // We don't do that here because we don't have TIME. This CPU is slow for
    //  the job.
    results[channel_index++] += ((ADRESH << 8) + ADRESL);
    
    // Wait for timer to fire
    while(!PIR1bits.TMR2IF);
    
    // Start sampling the new channel
    ADCON0bits.ADGO = 1;
}

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    i2c_slave_manager_init();
    
    CLRWDT();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    // Stop the timer and set the interrupt handler
    TMR0_StopTimer();
    TMR0_SetInterruptHandler(tmr0_isr);
    TMR2_StopTimer();
    
    // Fire the timer, after the timer elapses the ADC will start working 
    TMR0H = 0x7C;
    TMR0L = 0x00;
    TMR0_StartTimer();
    
    // Start with I2C module off
    i2c1_driver_close();
    PIE1bits.SSP1IE = 0;
    SCL1_SetDigitalInput();
    SDA1_SetDigitalInput();
    
    while(1)
    {
        // Data ready, toggle the SDA and SCL lines from high to low
        //  as an indication that the ESP32 can now read from I2C
        if(data_ready)
        {
            // Set SCL and SDA low for 10 us
            SCL1_SetLow();
            SDA1_SetLow();
            SCL1_SetDigitalOutput();
            SCL1_SetDigitalOutput();
            __delay_us(10);
            
            // Enable I2C again and wait!
            data_ready = 0;
            i2c_slave_manager_init();
        }   
    }
}