#include "led_manager.h"
#include "config/cansat.h"
#include "esp_log.h"
#include "driver/ledc.h"

static const uint32_t duty_resolution = 16;
static const uint32_t duty_max = 65536;         // 2^duty_resolution
static const char* TAG = "led";

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

// Task
static StackType_t stack[LED_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

// State machine
typedef enum 
{
    NEXT,
    SLOW_ON,
    SLOW_OFF,
    FAST_ON,
    FAST_OFF,
    OFF
} state_machine_t;
static state_machine_t status = OFF;
static bool fade = false;

static void set_led_duty(float duty, bool fade, int fade_time)
{
    // Stop any fading currently active
    ledc_stop(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL, 0);

    if(fade)
    {
        ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL, (uint32_t)((duty*(float)duty_max)/100.0), fade_time);
        ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL, LEDC_FADE_NO_WAIT);
    }
    else
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL, (uint32_t)((duty*(float)duty_max)/100.0));
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL);   
    }
}

static void fade_isr(void* args)
{

}

static void led_task(void* args)
{
    uint32_t v;

    while(true)
    {
        // Wait forever for a task notification
        xTaskNotifyWait(0x0000, 0xFFFFFFFF, &v, portMAX_DELAY);
        ESP_LOGV(TAG, "Notif: %d", v);
        
        // New value for the state machine?
        if(v != NEXT) status = v;

        switch(status)
        {
            case NEXT:
                ESP_LOGE(TAG, "NEXT SHOULD NEVER HAPPEN!");
                break;

            case SLOW_ON:
                set_led_duty(100, fade, LED_MANAGER_SLOW_BLINK_TIME);
                status = SLOW_OFF;
                break;

            case SLOW_OFF:
                set_led_duty(0, fade, LED_MANAGER_SLOW_BLINK_TIME);
                status = SLOW_ON;
                break;

            case FAST_ON:
                set_led_duty(100, fade, LED_MANAGER_FAST_BLINK_TIME);
                status = FAST_OFF;
                break;

            case FAST_OFF:
                set_led_duty(0, fade, LED_MANAGER_FAST_BLINK_TIME);
                status = FAST_ON;
                break;

            case OFF:
                ledc_stop(LEDC_HIGH_SPEED_MODE, LED_MANAGER_LED_CHANNEL, 0);
                break;
        }
    }
}

esp_err_t led_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    esp_err_t err;

    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Set Timer 0 to generate 1 kHz PWM
    ledc_timer_config_t timer_conf;
	timer_conf.duty_resolution = duty_resolution;
	timer_conf.freq_hz = 1000;
	timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num  = LEDC_TIMER_1;
	if((err = ledc_timer_config(&timer_conf)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting PWM timer");
        return err;
    }

    // Setup the channel for the LED
    ledc_channel_config_t ledc_conf;
	ledc_conf.channel = LED_MANAGER_LED_CHANNEL;
	ledc_conf.duty = 0;
	ledc_conf.gpio_num = LED_PIN;
	ledc_conf.intr_type = LEDC_INTR_FADE_END;
	ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel = LEDC_TIMER_1;
    ledc_conf.hpoint = 0;
	if((err = ledc_channel_config(&ledc_conf)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting channel LED_MANAGER_LED_CHANNEL");
        return err;
    }

    if(ledc_fade_func_install(0) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing fade function");
        return err;
    }

    // Set ISR
    ledc_isr_register(fade_isr, NULL, 0, NULL);

    task_handle = xTaskCreateStaticPinnedToCore(led_task, "led", LED_MANAGER_STACK_SIZE, 
        NULL, LED_MANAGER_TASK_PRIORITY, stack, &task, LED_MANAGER_AFFINITY);
    if(task_handle == NULL)
    {
        ESP_LOGE(TAG, "Error creating task");
        return ESP_FAIL;
    }

    led_manager_off();

    ESP_LOGI(TAG, "Ready!");
    return ESP_OK;
}

void led_manager_off(void)
{
    xTaskNotify(task_handle, OFF, eSetValueWithOverwrite);
}

void led_manager_slow_blink(bool faded)
{
    if(xSemaphoreTake(mutex, 100 / portTICK_PERIOD_MS))
    {
        fade = faded;

        // Notify the task that a new action needs to be performed
        xTaskNotify(task_handle, SLOW_ON, eSetValueWithOverwrite);
        xSemaphoreGive(mutex);
    }
}

void led_manager_fast_blink(bool faded)
{
    if(xSemaphoreTake(mutex, 100 / portTICK_PERIOD_MS))
    {
        fade = faded;

        // Notify the task that a new action needs to be performed
        xTaskNotify(task_handle, FAST_ON, eSetValueWithOverwrite);
        xSemaphoreGive(mutex);
    }
}