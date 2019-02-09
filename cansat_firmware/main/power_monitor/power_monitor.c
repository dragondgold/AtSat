#include "power_monitor.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "i2c_manager/i2c_manager.h"
#include "aux_ps/aux_ps.h"
#include "battery_manager/battery_manager.h"
#include "driver/i2c.h"
#include "config/cansat.h"
#include "esp_timer.h"

//#define LOG_LOCAL_LEVEL     ESP_LOG_VERBOSE
#include "esp_log.h"

static const char* TAG = "power";

// Task
static StackType_t stack[POWER_MONITOR_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

// I2C
static i2c_config_t pic16_i2c_config;

// Data
static const float v3v3_factor = 1.0/0.957;
static const float i3v3_factor = 1.0/6.0;
static const float v5v_factor = 1.0/0.639;
static const float i5v_factor = 1.0/10.0;
static const float ibat_factor = 1.0/2.55;
static power_status_t power_status;
static int64_t t2, t1;
static unsigned int fail_count = 0;

static uint16_t apply_filter(filter_data_t* filter, uint16_t new_value)
{
    signed int v = ((float)filter->last_filtered_value + filter->constant * (float)((signed int)new_value - (signed int)filter->last_filtered_value));
    v = v < 0 ? 0 : v;
    filter->last_filtered_value = v;

    return (uint16_t)v;
}

static uint16_t raw_to_measurement(uint16_t raw, float factor)
{
    // Convert to voltage or current, apply the factor then scale to mV or mA
    return (uint16_t)((((float)raw * ((float)POWER_MONITOR_VREF/(float)POWER_MONITOR_ADC_COUNTS)) * factor) * 1000.0);
}

// ISR will be executed on a falling edge from SDA
static void IRAM_ATTR scl_isr_handler(void* arg)
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(!gpio_get_level(SCL_PIC16_PIN))
    {
        // Notify the task to start the I2C transmission
        vTaskNotifyGiveFromISR(task_handle, &xHigherPriorityTaskWoken);
        t1 = esp_timer_get_time();

        if(xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }

    // FAILED!
}

static void power_monitor_task(void* args)
{
    uint8_t data[10] = {0};
    esp_err_t err = ESP_OK;

    while(true)
    {
        // Wait forever for a notification, if we were notified then SCL went low
        //  and we should start the I2C transmission now
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
        {
            // If we took more than 2 ms to respond to the interrupt, wait the next
            //  one because the read will probably fail
            t2 = esp_timer_get_time();
            if(t2 - t1 < 2000)
            {
                fail_count = 0;

                // Read the data from the PIC16. Disable interrupts first so they are not fired
                //  by the I2C activity
                gpio_intr_disable(SCL_PIC16_PIN);
                
                // The command has to be re-created every time, yes, not the best but that's what we have
                i2c_cmd_handle_t cmd = i2c_cmd_link_create();
                i2c_master_start(cmd);
                i2c_master_write_byte(cmd, POWER_MONITOR_PIC16_ADDRESS | 0x01, true);
                i2c_master_read(cmd, data, 10, I2C_MASTER_LAST_NACK);
                i2c_master_stop(cmd);

                err = i2c_master_cmd_begin(PIC16_I2C_NUMBER, cmd, pdMS_TO_TICKS(20));
                i2c_cmd_link_delete(cmd);

                gpio_intr_enable(SCL_PIC16_PIN);

                if(err != ESP_OK)
                {
                    ESP_LOGW(TAG, "Error reading");
                }
                else
                {
                    power_status.rail_3v3.raw_v = (((uint16_t)data[0] << 8) | (uint16_t)data[1]) / 13;
                    power_status.rail_5v.raw_v = (((uint16_t)data[2] << 8) | (uint16_t)data[3]) / 13;
                    power_status.rail_3v3.raw_i = (((uint16_t)data[4] << 8) | (uint16_t)data[5]) / 13;
                    power_status.rail_5v.raw_i = (((uint16_t)data[6] << 8) | (uint16_t)data[7]) / 13;
                    power_status.rail_bat.raw_i = (((uint16_t)data[8] << 8) | (uint16_t)data[9]) / 13;

                    power_status.rail_3v3.voltage = raw_to_measurement(power_status.rail_3v3.raw_v, v3v3_factor);
                    power_status.rail_3v3.current = raw_to_measurement(power_status.rail_3v3.raw_i, i3v3_factor);
                    power_status.rail_5v.voltage = raw_to_measurement(power_status.rail_5v.raw_v, v5v_factor);
                    power_status.rail_5v.current = raw_to_measurement(power_status.rail_5v.raw_i, i5v_factor);
                    power_status.rail_bat.current = raw_to_measurement(power_status.rail_bat.raw_i, ibat_factor);

                    power_status.rail_3v3.avg_voltage = apply_filter(&power_status.rail_3v3._filter_voltage,  power_status.rail_3v3.voltage);
                    power_status.rail_3v3.avg_current = apply_filter(&power_status.rail_3v3._filter_current,  power_status.rail_3v3.current);
                    power_status.rail_5v.avg_voltage = apply_filter(&power_status.rail_5v._filter_voltage, power_status.rail_5v.voltage);
                    power_status.rail_5v.avg_current = apply_filter(&power_status.rail_5v._filter_current, power_status.rail_5v.current);
                    power_status.rail_bat.avg_voltage = battery_manager_get().volts;
                    power_status.rail_bat.avg_current = apply_filter(&power_status.rail_bat._filter_current, power_status.rail_bat.current);

                    ESP_LOGV(TAG, "3.3 V: %d", power_status.rail_3v3.avg_voltage);
                    ESP_LOGV(TAG, "3.3 I: %d", power_status.rail_3v3.avg_current);
                    ESP_LOGV(TAG, "5 V: %d", power_status.rail_5v.avg_voltage);
                    ESP_LOGV(TAG, "5 I: %d", power_status.rail_5v.avg_current);
                    ESP_LOGV(TAG, "BAT I: %d", power_status.rail_bat.avg_current);

                    // Check currents
                    if(power_status.rail_3v3.avg_current > POWER_MONITOR_MAX_3V3_CURRENT)
                    {
                        power_status.rail_3v3.overcurrent = true;
                        aux_ps_disable();
                    }
                    if(power_status.rail_5v.avg_current > POWER_MONITOR_MAX_5V_CURRENT)
                    {
                        power_status.rail_5v.overcurrent = true;
                        aux_ps_disable();
                    }
                    if(power_status.rail_bat.avg_current > POWER_MONITOR_MAX_BAT_CURRENT)
                    {
                        power_status.rail_bat.overcurrent = true;
                        aux_ps_disable();
                    }

                    // Check voltages
                    if(power_status.rail_3v3.avg_voltage > POWER_MONITOR_MAX_3V3_VOLTAGE)
                    {
                        power_status.rail_3v3.overvoltage = true;
                        aux_ps_disable();
                    }
                    if(power_status.rail_5v.avg_voltage > POWER_MONITOR_MAX_5V_VOLTAGE)
                    {
                        power_status.rail_5v.overvoltage = true;
                        aux_ps_disable();
                    }
                    if(power_status.rail_bat.avg_voltage > POWER_MONITOR_MAX_BAT_VOLTAGE)
                    {
                        power_status.rail_bat.overvoltage = true;
                        aux_ps_disable();
                    }
                }
            }
            else
            {
                // Failed 10 times to serve the interrupt and read the values in time?
                // Disable PSU because we can't protect them
                if(fail_count == 10)
                {
                    aux_ps_disable();
                    ESP_LOGE(TAG, "Error on PIC16 communication");
                }
            }
            
        }
        else
        {
            ESP_LOGE(TAG, "Timeout should never occur");
        }
    }
}

esp_err_t power_monitor_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    fail_count = 0;
    // 3.3V
    power_status.rail_3v3._filter_current.constant = 0.5;
    power_status.rail_3v3._filter_current.last_filtered_value = 0;
    power_status.rail_3v3._filter_voltage.constant = 0.5;
    power_status.rail_3v3._filter_voltage.last_filtered_value = 0;
    // 5V
    power_status.rail_5v._filter_current.constant = 0;
    power_status.rail_5v._filter_current.last_filtered_value = 0;
    power_status.rail_5v._filter_voltage.constant = 0;
    power_status.rail_5v._filter_voltage.last_filtered_value = 0;
    // Bat
    power_status.rail_bat._filter_current.constant = 0;
    power_status.rail_bat._filter_current.last_filtered_value = 0;

    // Set I2C bus for the PIC16 at 400 kHz. At startup start with I2C disabled
    pic16_i2c_config.mode = I2C_MODE_MASTER;
    pic16_i2c_config.sda_io_num = SDA_PIC16_PIN;
    pic16_i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
    pic16_i2c_config.scl_io_num = SCL_PIC16_PIN;
    pic16_i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
    pic16_i2c_config.master.clk_speed = 400000;

    // Setup I2C
    if((err = i2c_param_config(PIC16_I2C_NUMBER, &pic16_i2c_config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting up I2C");
        return err;
    }

    // Install I2C driver
    if((err = i2c_driver_install(PIC16_I2C_NUMBER, pic16_i2c_config.mode, 0, 0, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing I2C driver");
        return err;
    }

    // Set SDA and SCL pins as inputs and enable interrupt so we can detect when the PIC16
    //  brings them low to alert us that new data is available.
    gpio_config_t config;
    config.pin_bit_mask = (1 << SDA_PIC16_PIN);
    config.mode = GPIO_MODE_DEF_INPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring SDA pin");
        return err;
    }

    config.pin_bit_mask = (1 << SCL_PIC16_PIN);
    config.mode = GPIO_MODE_DEF_INPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_NEGEDGE;
    if((err = gpio_config(&config)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error configuring SCL pin");
        return err;
    }

    // Task
    task_handle = xTaskCreateStaticPinnedToCore(power_monitor_task, "power", POWER_MONITOR_STACK_SIZE, 
        NULL, POWER_MONITOR_TASK_PRIORITY, stack, &task, POWER_MONITOR_AFFINITY);

    // Add SDA isr callback
    if((err = gpio_isr_handler_add(SCL_PIC16_PIN, scl_isr_handler, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting ISR handler");
        return err;
    }

    ESP_LOGI(TAG, "Ready!");
    return ESP_OK;
}

void power_monitor_clear_errors(void)
{
    fail_count = 0;
    power_status.rail_3v3.overcurrent = false;
    power_status.rail_3v3.overvoltage = false;
    power_status.rail_5v.overcurrent = false;
    power_status.rail_5v.overvoltage = false;
    power_status.rail_bat.overcurrent = false;
    power_status.rail_bat.overvoltage = false;
}

uint16_t power_monitor_get_battery_current(void)
{
    return power_status.rail_bat.avg_current;
}
uint16_t power_monitor_get_battery_voltage(void)
{
    return power_status.rail_bat.avg_voltage;
}
uint16_t power_monitor_get_3v3_current(void)
{
    return power_status.rail_3v3.avg_current;
}
uint16_t power_monitor_get_3v3_voltage(void)
{
    return power_status.rail_3v3.avg_voltage;
}
uint16_t power_monitor_get_5v_current(void)
{
    return power_status.rail_5v.avg_current;
}
uint16_t power_monitor_get_5v_voltage(void)
{
    return power_status.rail_5v.avg_voltage;
}
