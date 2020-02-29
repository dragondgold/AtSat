#include "adc_manager.h"
#include "rtc_manager.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "app_error.h"

#define NRF_LOG_MODULE_NAME adc_manager
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

static nrf_ppi_channel_t m_ppi_channel;
static rtc_event_t rtc_event;

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex = NULL;

static volatile adc_manager_cfg_t * current_cfg;

static void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        rtc_manager_event_done_from_isr(&rtc_event);

        // ADC has ended sampling, call the handler function
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        current_cfg->sampling_done_handler();

        xSemaphoreGiveFromISR(mutex, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

ret_code_t adc_manager_init(void)
{
    // Configure the ADC channel to sample the VDD voltage
    nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
    channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL;
    channel_config.gain = SAADC_CH_CONFIG_GAIN_Gain1_6;

    // Initialize the ADC
    ret_code_t err_code;
    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    // Initialize ADC channel
    err_code = nrf_drv_saadc_channel_init(ADC_MANAGER_BATTERY_CHANNEL, &channel_config);
    APP_ERROR_CHECK(err_code);

    // Setup ppi channel so that timer compare event is triggering sample task in SAADC
    uint32_t rtc_compare_event_addr = rtc_manager_get_event_address();
    uint32_t rtc_clear_task_addr = rtc_manager_get_clear_task_address();
    uint32_t saadc_sample_task_addr = nrf_drv_saadc_sample_task_get();

    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel, rtc_compare_event_addr, saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_fork_assign(m_ppi_channel, rtc_clear_task_addr);
    APP_ERROR_CHECK(err_code);

    // Enable PPI channel
    err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    // Create mutex for resource access
    mutex = xSemaphoreCreateBinaryStatic(&mutex_buffer);
    xSemaphoreGive(mutex);

    NRF_LOG_INFO("ADC configured");
    return err_code;
}

ret_code_t adc_manager_start_battery_sample(adc_manager_cfg_t * cfg)
{
    current_cfg = cfg;
    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(300))) 
    {
        // Request an event to sample the ADC
        rtc_event.before_start = NULL;
        rtc_event.elapsed = NULL;
        rtc_event.value = 64;
        rtc_event.is_ppi = true;

        if(rtc_manager_request_event(&rtc_event, 300))
        {
            return nrf_drv_saadc_buffer_convert((nrf_saadc_value_t *)current_cfg->samples, ADC_MANAGER_BATTERY_SAMPLES);
        }
        else
        {
            NRF_LOG_DEBUG("Failed requesting ADC event");
            return NRF_ERROR_INTERNAL;
        }
    }
    else
    {
        NRF_LOG_DEBUG("Failed to acquire ADC mutex");
    }

    return NRF_ERROR_BUSY;
}