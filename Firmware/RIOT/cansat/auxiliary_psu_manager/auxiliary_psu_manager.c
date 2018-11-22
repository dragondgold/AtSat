#include "auxiliary_psu_manager.h"
#include "config/cansat.h"
#include <stdbool.h>

#include "periph/gpio.h"
#include "periph/adc.h"
#include "mutex.h"
#include "xtimer.h"
#include "periph_conf.h"
#include "drivers/adc.h"

static mutex_t lock;
static bool is_psu_on = false;

static void auxiliary_psu_manager_adc_init(void) {
    // Enable ADC12 clock
    periph_clk_en(AHB, RCC_AHBENR_ADC12EN);

	// Set ADC pins in analog mode
    gpio_init_analog(GPIO_PIN(I_SENSE_5V_PORT, I_SENSE_5V_PIN));
    gpio_init_analog(GPIO_PIN(V_SENSE_5V_PORT, V_SENSE_5V_PIN));
    gpio_init_analog(GPIO_PIN(I_SENSE_3V3_PORT, I_SENSE_3V3_PIN));
    gpio_init_analog(GPIO_PIN(V_SENSE_3V3_PORT, V_SENSE_3V3_PIN));
    gpio_init_analog(GPIO_PIN(I_SENSE_BATT_PORT, I_SENSE_BATT_PIN));

    // ADC1 setup
	adc_power_off(ADC1);
    adc_enable_regulator(ADC1);
	adc_set_clk_prescale(ADC1, ADC_CCR_CKMODE_DIV2);    // 72 MHz / 2 = 36 MHz
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
    adc_set_resolution(ADC1, ADC_RES_12BIT);

    // ADC2 setup
	adc_power_off(ADC2);
    adc_enable_regulator(ADC2);
	adc_set_clk_prescale(ADC2, ADC_CCR_CKMODE_DIV2);    // 72 MHz / 2 = 36 MHz
	adc_set_single_conversion_mode(ADC2);
	adc_disable_external_trigger_regular(ADC2);
	adc_set_right_aligned(ADC2);
    adc_set_resolution(ADC2, ADC_RES_12BIT);

    /**
     * MCP6001 are used to buffer the voltage readings to the ADC inputs. The shortcircuit
     *  current at 3.3V is roughly 15 mA, that is an ouput impedance of 220 Ohm
     * In the case of current sensing, INA194 are used which have an output impedance of
     *  1.5 Ohms at 12V, we do a linear estimate saying that at 3.3V the output impedance
     *  is around 5.5 Ohm
     * The typical value for the sample and hold capacitor is 5 pF. If we consider 5 RC time
     *  constants, the minimum sampling time should be:
     * 
     * Voltage -> 5 * 5 pF * 220 Ohm = 5.5 ns
     * Current -> 5 * 5 pF * 5.5 Ohm = 0.14 ns
     * 
     * With a 36 MHz clock on the ADC module, the period is ~28 ns, so even a single cycle is
     *  enough as sample time. 21 ns is the minimun sample time allowed by the ADC.
     */
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_2DOT5CYC);   // 64 ns
    adc_set_sample_time_on_all_channels(ADC2, ADC_SMPR_SMP_2DOT5CYC);   // 64 ns

	// Set all the channels that will be converted on each trigger
	uint8_t channel_array_adc1[] = { I_SENSE_5V_CHANNEL, I_SENSE_3V3_PIN, I_SENSE_BATT_CHANNEL };
    uint8_t channel_array_adc2[] = { V_SENSE_5V_CHANNEL, V_SENSE_3V3_CHANNEL };
	adc_set_regular_sequence(ADC1, 1, channel_array_adc1);
    adc_set_regular_sequence(ADC1, 1, channel_array_adc2);

	// Regulator startup time (10 us worst case scenario)
    xtimer_usleep(10);

    // Calibrate the ADC
    adc_calibrate(ADC1);

    adc_power_on(ADC1);
    adc_enable_eoc_interrupt(ADC1);
    
    // Wait at least 1 us for ADC stabilization
    xtimer_usleep(5);
}

void auxiliary_psu_manager_init(void) 
{
    mutex_init(&lock);

    // Init ADC1 and ADC2
    auxiliary_psu_manager_adc_init();

    is_psu_on = false;
    auxiliary_psu_manager_turn_off();
}

void auxiliary_psu_manager_turn_on(void) 
{
    is_psu_on = true;
    gpio_set(GPIO_PIN(ENABLE_5V_PORT, ENABLE_5V_PIN));
    gpio_set(GPIO_PIN(ENABLE_3V3_PORT, ENABLE_3V3_PIN));
}

void auxiliary_psu_manager_turn_off(void)
{
    is_psu_on = false;
    gpio_clear(GPIO_PIN(ENABLE_5V_PORT, ENABLE_5V_PIN));
    gpio_clear(GPIO_PIN(ENABLE_3V3_PORT, ENABLE_3V3_PIN));
}

bool auxiliary_psu_manager_is_on(void)
{
    return is_psu_on;
}

void auxiliary_psu_manager_acquire(void)
{
    mutex_lock(&lock);
}

void auxiliary_psu_manager_release(void)
{
    mutex_unlock(&lock);
}