/**
 * @ingroup     cpu_stm32f3
 * @ingroup     drivers_periph_adc
 * @{
 *
 * @file
 * @brief       Low-level ADC driver implementation
 *
 * @author      Andres Torti <torti.max@gmail.com>
 *
 * This code is based on the amazing libopencm3 library (https://github.com/libopencm3/libopencm3)
 *  and adapted to be used with RIOT OS to be thread-safe!
 * @}
 */

#include "cpu.h"
#include "mutex.h"
#include "periph/adc.h"
#include "periph_conf.h"
#include "adc.h"

/**
 * TODO: This defines should be in periph_conf.h of the cansat board, but,
 *  for some reason, it doesn't get included when puting them in that file
 * @name   ADC configuration
 * @{
 */
#define ADC_DEVS           (4U)
#define ADC_NUMOF          (5U)
// PIN, ADC DEVICE, ADC CHANNEL
#define ADC_CONFIG {                \
    { GPIO_PIN(PORT_A, 0), 0, 0 },  \
    { GPIO_PIN(PORT_A, 1), 0, 1 },  \
    { GPIO_PIN(PORT_A, 4), 0, 4 },  \
    { GPIO_PIN(PORT_B, 0), 0, 8 },  \
    { GPIO_PIN(PORT_C, 1), 0, 11 }, \
}
/** @} */

/**
 * @brief   Load the ADC configuration
 */
static const adc_conf_t adc_config[] = ADC_CONFIG;

/**
 * @brief   Allocate locks for all three available ADC devices
 */
static mutex_t locks[] = {
#if ADC_DEVS > 1
    MUTEX_INIT,
#endif
#if ADC_DEVS > 2
    MUTEX_INIT,
#endif
#if ADC_DEVS > 3
    MUTEX_INIT,
#endif
    MUTEX_INIT
};

static inline ADC_TypeDef *dev(adc_t line)
{
    return (ADC_TypeDef *)(ADC1_BASE + (adc_config[line].dev << 8));
}

static inline void prep(adc_t line)
{
    mutex_lock(&locks[adc_config[line].dev]);
}

static inline void done(adc_t line)
{
    mutex_unlock(&locks[adc_config[line].dev]);
}

void adc_acquire(ADC_TypeDef* adc) {
    if(adc == ADC1) mutex_lock(&locks[0]);
    else if(adc == ADC2) mutex_lock(&locks[1]);
    else if(adc == ADC3) mutex_lock(&locks[2]);
    else if(adc == ADC4) mutex_lock(&locks[3]);
}

void adc_release(ADC_TypeDef* adc) {
    if(adc == ADC1) mutex_unlock(&locks[0]);
    else if(adc == ADC2) mutex_unlock(&locks[1]);
    else if(adc == ADC3) mutex_unlock(&locks[2]);
    else if(adc == ADC4) mutex_unlock(&locks[3]);
}

/** @brief ADC Read the End-of-Conversion Flag
 *
 * This flag is set by hardware at the end of each regular conversion of a
 * channel when a new data is available in the ADCx_DR register.
 *
 * @param[in] adc Unsigned int32. ADC block register address base
 * @ref adc_reg_base
 * @returns bool. End of conversion flag.
 */
bool adc_eoc(ADC_TypeDef* adc)
{
    return adc->ISR & ADC_ISR_EOC;
}

/** @brief ADC Read the End-of-Sequence Flag for Regular Conversions
 *
 * This flag is set after all channels of an regular group have been
 * converted.
 *
 * @param[in] adc Unsigned int32. ADC block register address base
 * @ref adc_reg_base
 * @returns bool. End of conversion flag.
 */
bool adc_eos(ADC_TypeDef* adc)
{
	return adc->ISR & ADC_ISR_EOS;
}

/**
 * Turn on the ADC (async)
 * @sa adc_wait_power_on
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_power_on_async(ADC_TypeDef* adc)
{
    adc->CR |= ADC_CR_ADEN;
}

/**
 * Is the ADC powered up and ready?
 * @sa adc_power_on_async
 * @param adc ADC Block register address base @ref adc_reg_base
 * @return true if adc is ready for use
 */
bool adc_is_power_on(ADC_TypeDef* adc)
{
	return adc->ISR & ADC_ISR_ADRDY;
}

/**
 * Turn on the ADC
 * @sa adc_power_on_async
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_power_on(ADC_TypeDef* adc)
{
	adc_power_on_async(adc);
	while (!adc_is_power_on(adc));
}

/**
 * Turn off the ADC (async)
 * This will actually block if it needs to turn off a currently running
 * conversion, as per ref man. (Handles injected on hardware that supports
 * injected conversions.
 * @sa adc_wait_power_off
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_power_off_async(ADC_TypeDef* adc)
{
	if (adc_is_power_off(adc)) {
		return;
	}

	uint32_t checks = ADC_CR_ADSTART;
	uint32_t stops = ADC_CR_ADSTP;
#if defined(ADC_CR_JADSTART)
	checks |= ADC_CR_JADSTART;
	stops |= ADC_CR_JADSTP;
#endif
	if (adc->CR & checks) {
		adc->CR |= stops;
		while (adc->CR & checks);
	}
	adc->CR |= ADC_CR_ADDIS;
}

/**
 * Is the ADC powered down?
 * @sa adc_power_off_async
 * @param adc ADC Block register address base @ref adc_reg_base
 */
bool adc_is_power_off(ADC_TypeDef* adc)
{
	return !(adc->CR & ADC_CR_ADEN);
}

/**
 * Turn off the ADC
 * This will actually block if it needs to turn off a currently running
 * conversion, as per ref man.
 * @sa adc_power_off_async
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_power_off(ADC_TypeDef* adc)
{
	adc_power_off_async(adc);
	while (!adc_is_power_off(adc));
}

/**
 * Start the ADC calibration and immediately return.
 * @sa adc_calibrate
 * @sa adc_is_calibrating
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_calibrate_async(ADC_TypeDef* adc)
{
	adc->CR = ADC_CR_ADCAL;
}

/**
 * Is the ADC Calibrating?
 * @param adc ADC Block register address base @ref adc_reg_base
 * @return true if the adc is currently calibrating
 */
bool adc_is_calibrating(ADC_TypeDef* adc)
{
	return adc->CR & ADC_CR_ADCAL;
}

/**
 * Start ADC calibration and wait for it to finish
 * @param adc ADC Block register address base @ref adc_reg_base
 */
void adc_calibrate(ADC_TypeDef* adc)
{
	adc_calibrate_async(adc);
	while (adc_is_calibrating(adc));
}

/**
 * Enable Continuous Conversion Mode
 * In this mode the ADC starts a new conversion of a single channel or a channel
 * group immediately following completion of the previous channel group
 * conversion.
 *
 * @param[in] adc ADC block register address base @ref adc_reg_base
 */
void adc_set_continuous_conversion_mode(ADC_TypeDef* adc)
{
	adc->CFGR |= ADC_CFGR_CONT;
}

/**
 * Enable Single Conversion Mode
 * In this mode the ADC performs a conversion of one channel or a channel group
 * and stops.
 *
 * @param[in] adc ADC block register address base @ref adc_reg_base
 */
void adc_set_single_conversion_mode(ADC_TypeDef* adc)
{
	adc->CFGR &= ~ADC_CFGR_CONT;
}

/** @brief ADC Set Resolution
 *
 * ADC Resolution can be reduced from 12 bits to 10, 8 or 6 bits for a
 * corresponding reduction in conversion time.
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 * @param[in] resolution Unsigned int16. Resolution value (@ref adc_api_res)
 */
void adc_set_resolution(ADC_TypeDef* adc, adc_res_t resolution)
{
    uint32_t res;

    switch(resolution) {
        case ADC_RES_6BIT:
            res = (0x3 << 3);
            break;

        case ADC_RES_8BIT:
            res = (0x2 << 3);
            break;

        case ADC_RES_10BIT:
            res = (0x1 << 3);
            break;

        // 12-bit or more
        default:
            res = (0x0 << 3);
            break;
    }

	adc->CFGR = (adc->CFGR & ~ADC_CFGR_RES_Msk) | res;
}

/** @brief ADC Set the Sample Time for All Channels
 *
 * The sampling time can be selected in ADC clock cycles, exact values
 * depend on the device.
 *
 * @param[in] adc ADC block register address base @ref adc_reg_base
 * @param[in] time Sampling time selection from @ref adc_sample
 */
void adc_set_sample_time_on_all_channels(ADC_TypeDef* adc, adc_sample_time_t time)
{
	uint8_t i;
	uint32_t reg32 = 0;

	for (i = 0; i <= 9; i++) {
		reg32 |= ((uint32_t)time << (i * 3));
	}
	adc->SMPR1 = reg32;

	for (i = 10; i <= 17; i++) {
		reg32 |= ((uint32_t)time << ((i - 10) * 3));
	}
	adc->SMPR2 = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief ADC Set a Regular Channel Conversion Sequence
 *
 * Define a sequence of channels to be converted as a regular group with a
 * length from 1 to 16 channels. If this is called during conversion, the
 * current conversion is reset and conversion begins again with the newly
 * defined group.
 *
 * @param[in] adc ADC block register address base @ref adc_reg_base
 * @param[in] length Number of channels in the group, range 0..16
 * @param[in] channel Set of channels in sequence, range @ref adc_channel
 */
void adc_set_regular_sequence(ADC_TypeDef* adc, uint8_t length, uint8_t channel[])
{
	uint32_t reg32_1 = 0, reg32_2 = 0, reg32_3 = 0, reg32_4 = 0;
	uint8_t i = 0;

	/* Maximum sequence length is 16 channels. */
	if (length > 16) {
		return;
	}

	for (i = 1; i <= length; i++) {
		if (i <= 4) {
			reg32_1 |= (channel[i - 1] << (i * 6));
		}
		if ((i > 4) & (i <= 9)) {
			reg32_2 |= (channel[i - 1] << ((i - 4 - 1) * 6));
		}
		if ((i > 9) & (i <= 14)) {
			reg32_3 |= (channel[i - 1] << ((i - 9 - 1) * 6));
		}
		if ((i > 14) & (i <= 16)) {
			reg32_4 |= (channel[i - 1] << ((i - 14 - 1) * 6));
		}
	}
	reg32_1 |= (length - 1);

	adc->SQR1 = reg32_1;
	adc->SQR2 = reg32_2;
	adc->SQR3 = reg32_3;
	adc->SQR4 = reg32_4;
}

/** @brief ADC Set the Data as Left Aligned
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_set_left_aligned(ADC_TypeDef* adc)
{
	adc->CFGR |= ADC_CFGR_ALIGN;
}

/** @brief ADC Set the Data as Right Aligned
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_set_right_aligned(ADC_TypeDef* adc)
{
	adc->CFGR &= ~ADC_CFGR_ALIGN;
}

/** @brief ADC Enable DMA Transfers
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_enable_dma(ADC_TypeDef* adc)
{
	adc->CFGR |= ADC_CFGR_DMAEN;
}

/** @brief ADC Disable DMA Transfers
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_disable_dma(ADC_TypeDef* adc)
{
	adc->CFGR &= ~ADC_CFGR_DMAEN;
}

/** @brief ADC Enable the Overrun Interrupt
 *
 * The overrun interrupt is generated when data is not read from a result
 * register before the next conversion is written. If DMA is enabled, all
 * transfers are terminated and any conversion sequence is aborted.
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_enable_overrun_interrupt(ADC_TypeDef* adc)
{
	adc->IER |= ADC_IER_OVRIE;
}

/** @brief ADC Disable the Overrun Interrupt
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_disable_overrun_interrupt(ADC_TypeDef* adc)
{
	adc->IER &= ~ADC_IER_OVRIE;
}

/** @brief ADC Read the Overrun Flag
 *
 * The overrun flag is set when data is not read from a result register before
 * the next conversion is written. If DMA is enabled, all transfers are
 * terminated and any conversion sequence is aborted.
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
bool adc_get_overrun_flag(ADC_TypeDef* adc)
{
	return adc->ISR & ADC_ISR_OVR;
}

/** @brief ADC Clear Overrun Flags
 *
 * The overrun flag is cleared. Note that if an overrun occurs, DMA is
 * terminated.
 * The flag must be cleared and the DMA stream and ADC reinitialised to resume
 * conversions (see the reference manual).
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_clear_overrun_flag(ADC_TypeDef* adc)
{
	adc->ISR = ADC_ISR_OVR;
}

/** @brief ADC Enable Regular End-Of-Conversion Interrupt
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_enable_eoc_interrupt(ADC_TypeDef* adc)
{
	adc->IER |= ADC_IER_EOCIE;
}

/** @brief ADC Disable Regular End-Of-Conversion Interrupt
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_disable_eoc_interrupt(ADC_TypeDef* adc)
{
	adc->IER &= ~ADC_IER_EOCIE;
}

/** @brief ADC Read from the Regular Conversion Result Register
 *
 * The result read back is 12 bits, right or left aligned within the first
 * 16 bits.
 *
 * @param[in] adc Unsigned int32. ADC block register address base
 * @ref adc_reg_base
 * @returns Unsigned int32 conversion result.
 */
uint32_t adc_read_regular(ADC_TypeDef* adc)
{
	return adc->DR;
}

/**
 * Enable the temperature sensor (only)
 * The channel this is available on is unfortunately not
 * consistent, even though the bit used to enable it is.
 * @sa adc_disable_temperature_sensor
 */
void adc_enable_temperature_sensor(ADC_TypeDef* adc)
{
    if(adc == ADC1 || adc == ADC2) {
        ADC12_COMMON->CCR |= ADC_CCR_TSEN;
    }
    else if(adc == ADC3 || adc == ADC4) {
        ADC34_COMMON->CCR |= ADC_CCR_TSEN;
    }
}

/**
 * Disable the temperature sensor (only)
 * @sa adc_enable_temperature_sensor
 */
void adc_disable_temperature_sensor(ADC_TypeDef* adc)
{
    if(adc == ADC1 || adc == ADC2) {
        ADC12_COMMON->CCR &= ~ADC_CCR_TSEN;
    }
    else if(adc == ADC3 || adc == ADC4) {
        ADC34_COMMON->CCR &= ~ADC_CCR_TSEN;
    }
}

/**
 * Enable the internal voltage reference (only)
 * The channel this is available on is unfortunately not
 * consistent, even though the bit used to enable it is.
 * FIXME - on f3, you can actually have it on ADC34 as well!
 * @sa adc_disable_vrefint
 */
void adc_enable_vrefint(ADC_TypeDef* adc)
{
    if(adc == ADC1 || adc == ADC2) {
        ADC12_COMMON->CCR |= ADC_CCR_VREFEN;
    }
    else if(adc == ADC3 || adc == ADC4) {
        ADC34_COMMON->CCR |= ADC_CCR_VREFEN;
    }
}

/**
 * Disable the internal voltage reference (only)
 * @sa adc_enable_vrefint
 */
void adc_disable_vrefint(ADC_TypeDef* adc)
{
    if(adc == ADC1 || adc == ADC2) {
        ADC12_COMMON->CCR &= ~ADC_CCR_VREFEN;
    }
    else if(adc == ADC3 || adc == ADC4) {
        ADC34_COMMON->CCR &= ~ADC_CCR_VREFEN;
    }
}

/** @brief ADC Software Triggered Conversion on Regular Channels
 *
 * This starts conversion on a set of defined regular channels.
 * Depending on the configuration bits EXTEN, a conversion will start
 * immediately (software trigger configuration) or once a regular hardware
 * trigger event occurs (hardware trigger configuration)
 *
 * @param[in] adc ADC block register address base @ref adc_reg_base
 */
void adc_start_conversion_regular(ADC_TypeDef* adc)
{
	/* Start conversion on regular channels. */
	adc->CR |= ADC_CR_ADSTART;
}

/** @brief Enable circular mode for DMA transfers
 *
 * For this to work it needs to be ebabled on the DMA side as well.
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_enable_dma_circular_mode(ADC_TypeDef* adc)
{
	adc->CFGR |= ADC_CFGR_DMACFG;
}

/** @brief Disable circular mode for DMA transfers
 *
 * @param[in] adc Unsigned int32. ADC base address (@ref adc_reg_base)
 */
void adc_disable_dma_circular_mode(ADC_TypeDef* adc)
{
	adc->CFGR &= ~ADC_CFGR_DMACFG;
}

/**
 * Enable the ADC Voltage regulator
 * Before any use of the ADC, the ADC Voltage regulator must be enabled.
 * You must wait up to 10uSecs afterwards before trying anything else.
 * @param[in] adc ADC block register address base
 * @sa adc_disable_regulator
 */
void adc_enable_regulator(ADC_TypeDef* adc)
{
	adc->CR &= ~ADC_CR_ADVREGEN_Msk;
	adc->CR |= ADC_CR_ADVREGEN_1;
}

/**
 * Disable the ADC Voltage regulator
 * You can disable the adc vreg when not in use to save power
 * @param[in] adc ADC block register address base
 * @sa adc_enable_regulator
 */
void adc_disable_regulator(ADC_TypeDef* adc)
{
	adc->CR &= ~ADC_CR_ADVREGEN_Msk;
	adc->CR |= ADC_CR_ADVREGEN_0;
}

/*---------------------------------------------------------------------------*/
/** @brief ADC Set Clock Prescale
 *
 * The ADC clock taken from the APB2 clock can be scaled down by 2, 4, 6 or 8.
 *
 * @param[in] prescale Unsigned int32. Prescale value for ADC Clock @ref
 * adc_ccr_adcpre
*/
void adc_set_clk_prescale(ADC_TypeDef* adc, adc_clk_div_t prescale)
{
    ADC_Common_TypeDef* adc_group = NULL;
    if(adc == ADC1 || adc == ADC2) {
        adc_group = ADC12_COMMON;
    }
    else {
        adc_group = ADC34_COMMON;
    }

	uint32_t reg32 = ((adc_group->CCR & ~ADC_CCR_CKMODE_Msk) | (uint32_t)prescale);
	adc_group->CCR = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief ADC Disable an External Trigger for Regular Channels
 *
 * @param[in] adc Unsigned int32. ADC block register address base
 * @ref adc_reg_base
 */

void adc_disable_external_trigger_regular(ADC_TypeDef* adc)
{
	adc->CFGR &= ~ADC_CFGR_EXTEN_Msk;
}

/**
 * UNIMPLEMENTED
 */
int adc_init(adc_t line)
{
    return -1;
}

/**
 * UNIMPLEMENTED
 */
int adc_sample(adc_t line, adc_res_t res)
{
    return 0;
}
