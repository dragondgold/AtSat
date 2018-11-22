#ifndef CANSAT_ADC_H_
#define CANSAT_ADC_H_

typedef enum {
    /* CKMODE[1:0]: ADC clock mode */
    ADC_CCR_CKMODE_CKX  = (0x0 << 16),
    ADC_CCR_CKMODE_DIV1	= (0x1 << 16),
    ADC_CCR_CKMODE_DIV2	= (0x2 << 16),
    ADC_CCR_CKMODE_DIV4 = (0x3 << 16)
} adc_clk_div_t;

typedef enum {
    ADC_SMPR_SMP_1DOT5CYC   = 0x0,
    ADC_SMPR_SMP_2DOT5CYC   = 0x1,
    ADC_SMPR_SMP_4DOT5CYC   = 0x2,
    ADC_SMPR_SMP_7DOT5CYC   = 0x3,
    ADC_SMPR_SMP_19DOT5CYC  = 0x4,
    ADC_SMPR_SMP_61DOT5CYC  = 0x5,
    ADC_SMPR_SMP_181DOT5CYC = 0x6,
    ADC_SMPR_SMP_601DOT5CYC = 0x7
} adc_sample_time_t;

void adc_acquire(ADC_TypeDef* adc);
void adc_release(ADC_TypeDef* adc);

void adc_power_on_async(ADC_TypeDef* adc);
void adc_power_on(ADC_TypeDef* adc);
bool adc_is_power_on(ADC_TypeDef* adc);
void adc_power_off_async(ADC_TypeDef* adc);
void adc_power_off(ADC_TypeDef* adc);
bool adc_is_power_off(ADC_TypeDef* adc);
void adc_calibrate_async(ADC_TypeDef* adc);
bool adc_is_calibrating(ADC_TypeDef* adc);
void adc_calibrate(ADC_TypeDef* adc);
void adc_set_continuous_conversion_mode(ADC_TypeDef* adc);
void adc_set_single_conversion_mode(ADC_TypeDef* adc);
void adc_set_regular_sequence(ADC_TypeDef* adc, uint8_t length, uint8_t channel[]);
void adc_set_sample_time_on_all_channels(ADC_TypeDef* adc, adc_sample_time_t time);
void adc_enable_temperature_sensor(ADC_TypeDef* adc);
void adc_disable_temperature_sensor(ADC_TypeDef* adc);
void adc_enable_vrefint(ADC_TypeDef* adc);
void adc_disable_vrefint(ADC_TypeDef* adc);
void adc_set_resolution(ADC_TypeDef* adc, adc_res_t resolution);
void adc_set_left_aligned(ADC_TypeDef* adc);
void adc_set_right_aligned(ADC_TypeDef* adc);
void adc_enable_dma(ADC_TypeDef* adc);
void adc_disable_dma(ADC_TypeDef* adc);
bool adc_eoc(ADC_TypeDef* adc);
bool adc_eos(ADC_TypeDef* adc);
void adc_enable_eoc_interrupt(ADC_TypeDef* adc);
void adc_disable_eoc_interrupt(ADC_TypeDef* adc);
void adc_enable_overrun_interrupt(ADC_TypeDef* adc);
void adc_disable_overrun_interrupt(ADC_TypeDef* adc);
bool adc_get_overrun_flag(ADC_TypeDef* adc);
void adc_clear_overrun_flag(ADC_TypeDef* adc);
uint32_t adc_read_regular(ADC_TypeDef* adc);
void adc_start_conversion_regular(ADC_TypeDef* adc);
void adc_enable_dma_circular_mode(ADC_TypeDef* adc);
void adc_disable_dma_circular_mode(ADC_TypeDef* adc);
void adc_enable_regulator(ADC_TypeDef* adc);
void adc_disable_regulator(ADC_TypeDef* adc);
void adc_set_clk_prescale(ADC_TypeDef* adc, adc_clk_div_t prescale);
void adc_disable_external_trigger_regular(ADC_TypeDef* adc);

#endif