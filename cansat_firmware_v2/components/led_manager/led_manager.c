#include <limits.h>
#include <string.h>
#include "led_manager.h"
#include "nrf_drv_pwm.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define NRF_LOG_MODULE_NAME led_manager
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

static bool drv_initialized = false;
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);

static void generate_fade_values(nrf_pwm_values_common_t* values, uint16_t top_value, uint16_t steps_count, bool invert)
{
    uint16_t value = invert ? top_value : 0;
    uint16_t step = top_value / steps_count;
    for (uint8_t i = 0; i < steps_count; ++i)
    {
        if(invert) 
        {
            value -= step;
        }
        else 
        {
            value += step;
        }
        values[i] = value;
    }
}

static void set_led_state(bool state)
{
    if(drv_initialized)
    {
        nrf_drv_pwm_uninit(&m_pwm0);
        drv_initialized = false;
    }

    nrf_gpio_cfg_output(LED_MANAGER_LED_PIN);
    nrf_gpio_pin_write(LED_MANAGER_LED_PIN, state);
}

static void blink_led(bool faded, bool fast)
{
    if(drv_initialized)
    {
        nrf_drv_pwm_uninit(&m_pwm0);
        drv_initialized = false;
    }

    uint32_t repeats = fast ? LED_MANAGER_FAST_BLINK_REPEATS : LED_MANAGER_SLOW_BLINK_REPEATS;

    // Configure PWM to drive LED blinking sequence
    nrf_drv_pwm_config_t const config =
    {
        .output_pins =
        {
            LED_MANAGER_LED_PIN | NRF_DRV_PWM_PIN_INVERTED
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = LED_MANAGER_COUNTER_TOP_VALUE,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };

    // Initialize PWM
    nrfx_err_t err = nrf_drv_pwm_init(&m_pwm0, &config, NULL);
    if(err != NRFX_SUCCESS)
    {
        NRF_LOG_ERROR("Error initializing LED PWM");
        return;
    }
    drv_initialized = true;

    // This array cannot be allocated in the stack and it must be in RAM.
    // Generate the values for the fade-in LED sequence
    static nrf_pwm_values_common_t fade_in_seq_values[LED_MANAGER_PWM_STEPS];
    if(faded)
    {
        generate_fade_values(fade_in_seq_values, LED_MANAGER_COUNTER_TOP_VALUE, LED_MANAGER_PWM_STEPS, false);
    }
    else
    {
        memset(fade_in_seq_values, LED_MANAGER_COUNTER_TOP_VALUE, sizeof(fade_in_seq_values));
    }

    nrf_pwm_sequence_t const fade_in_seq =
    {
        .values.p_common = fade_in_seq_values,
        .length          = NRF_PWM_VALUES_LENGTH(fade_in_seq_values),
        .repeats         = repeats,
        .end_delay       = 0
    };

    // Now generate the values for the fade-out sequence
    static nrf_pwm_values_common_t fade_out_seq_values[LED_MANAGER_PWM_STEPS];
    if(faded)
    {
        generate_fade_values(fade_out_seq_values, LED_MANAGER_COUNTER_TOP_VALUE, LED_MANAGER_PWM_STEPS, true);
    }
    else
    {
        memset(fade_out_seq_values, 0, sizeof(fade_out_seq_values));
    }

    nrf_pwm_sequence_t const fade_out_seq =
    {
        .values.p_common = fade_out_seq_values,
        .length          = NRF_PWM_VALUES_LENGTH(fade_out_seq_values),
        .repeats         = repeats,
        .end_delay       = 0
    };

    // Start the sequence
    nrf_drv_pwm_complex_playback(&m_pwm0, &fade_in_seq, &fade_out_seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

bool led_manager_init(void)
{
    NRF_LOG_INFO("Initializing");

    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    NRF_LOG_INFO("Ready!");
    return true;
}

void led_manager_off(void)
{
    set_led_state(false);
}

void led_manager_on(void)
{
    set_led_state(true);
}

void led_manager_slow_blink(bool faded)
{
    if(xSemaphoreTake(mutex, LED_MANAGER_MUTEX_TIMEOUT))
    {
        blink_led(faded, false);
        xSemaphoreGive(mutex);
    }
    else
    {
        NRF_LOG_WARNING("Couldn't lock mutex on slow blink");
    }
}

void led_manager_fast_blink(bool faded)
{
    if(xSemaphoreTake(mutex, LED_MANAGER_MUTEX_TIMEOUT))
    {
        blink_led(faded, true);
        xSemaphoreGive(mutex);
    }
    else
    {
        NRF_LOG_WARNING("Couldn't lock mutex on fast blink");
    }
}