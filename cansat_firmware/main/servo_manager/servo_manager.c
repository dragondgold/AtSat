#include "servo_manager.h"
#include "config/cansat.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "math.h"

static const uint32_t duty_resolution = 16;
static const uint32_t duty_max = 65536;         // 2^duty_resolution
static const char* TAG = "servo";

// Mutex
static StaticSemaphore_t mutex_buffer;
static SemaphoreHandle_t mutex;

// Status
static bool balloon_opened = false, parachute_opened = false;

static inline void set_servo_duty(ledc_channel_t channel, float duty)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, (uint32_t)((duty*(float)duty_max)/100.0));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

esp_err_t servo_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing");

    esp_err_t err;

    // Set Timer 0 to generate 50 Hz PWM
    ledc_timer_config_t timer_conf;
	timer_conf.duty_resolution = duty_resolution;
	timer_conf.freq_hz = 50;
	timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num  = LEDC_TIMER_0;
	if((err = ledc_timer_config(&timer_conf)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting PWM timer");
        return err;
    }

    // Setup the channel for each of the two servos
    ledc_channel_config_t ledc_conf;
	ledc_conf.channel = SERVO_MANAGER_PARACHUTE_CHANNEL;
	ledc_conf.duty = 0;
	ledc_conf.gpio_num = SERVO_PARACHUTE_PIN;
	ledc_conf.intr_type = LEDC_INTR_DISABLE;
	ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel = LEDC_TIMER_0;
    ledc_conf.hpoint = 0;
	if((err = ledc_channel_config(&ledc_conf)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting channel SERVO_MANAGER_PARACHUTE_CHANNEL");
        return err;
    }

	ledc_conf.channel = SERVO_MANAGER_BALLOON_CHANNEL;
	ledc_conf.duty = 0;
	ledc_conf.gpio_num = SERVO_GLOBO_PIN;
	ledc_conf.intr_type = LEDC_INTR_DISABLE;
	ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel = LEDC_TIMER_0;
    ledc_conf.hpoint = 0;
    if((err = ledc_channel_config(&ledc_conf)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting channel SERVO_MANAGER_BALLOON_CHANNEL");
        return err;
    }

    // Create mutex for this resource
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);

    // Close at startup
    servo_manager_close_parachute();
    servo_manager_close_balloon();

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}

bool servo_manager_open_parachute(void)
{
    if(xSemaphoreTake(mutex, 200 / portTICK_PERIOD_MS))
    {
        set_servo_duty(SERVO_MANAGER_PARACHUTE_CHANNEL, SERVO_MANAGER_OPEN_SERVO_DUTY);
        parachute_opened = true;
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

bool servo_manager_close_parachute(void)
{
    if(xSemaphoreTake(mutex, 200 / portTICK_PERIOD_MS))
    {
        set_servo_duty(SERVO_MANAGER_PARACHUTE_CHANNEL, SERVO_MANAGER_CLOSE_SERVO_DUTY);
        parachute_opened = false;
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

bool servo_manager_open_balloon(void)
{
    if(xSemaphoreTake(mutex, 200 / portTICK_PERIOD_MS))
    {
        set_servo_duty(SERVO_MANAGER_BALLOON_CHANNEL, SERVO_MANAGER_OPEN_SERVO_DUTY);
        balloon_opened = true;
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

bool servo_manager_close_balloon(void)
{
    if(xSemaphoreTake(mutex, 200 / portTICK_PERIOD_MS))
    {
        set_servo_duty(SERVO_MANAGER_BALLOON_CHANNEL, SERVO_MANAGER_CLOSE_SERVO_DUTY);
        balloon_opened = false;
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

bool servo_manager_is_parachute_open(void)
{
    return parachute_opened;
}

bool servo_manager_is_ballon_open(void)
{
    return balloon_opened;
}