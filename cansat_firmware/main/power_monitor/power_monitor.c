#include "power_monitor.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "i2c_manager/i2c_manager.h"
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
static power_status_t power_status;

// ISR will be executed on a falling edge from SDA
static void IRAM_ATTR scl_isr_handler(void* arg)
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(!gpio_get_level(SCL_PIC16_PIN))
    {
        // Notify the task to start the I2C transmission
        vTaskNotifyGiveFromISR(task_handle, &xHigherPriorityTaskWoken);

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
                power_status.raw_3v3_v = ((uint16_t)data[0] << 8) | (uint16_t)data[1];
                power_status.raw_5v_v = ((uint16_t)data[2] << 8) | (uint16_t)data[3];
                power_status.raw_3v3_i = ((uint16_t)data[4] << 8) | (uint16_t)data[5];
                power_status.raw_5v_i = ((uint16_t)data[6] << 8) | (uint16_t)data[7];
                power_status.raw_bat_i = ((uint16_t)data[8] << 8) | (uint16_t)data[9];
                ESP_LOGV(TAG, "3.3V V: %d", power_status.raw_3v3_v/13);
                ESP_LOGV(TAG, "5V V: %d", power_status.raw_5v_v/13);
                ESP_LOGV(TAG, "3.3V I: %d", power_status.raw_3v3_i/13);
                ESP_LOGV(TAG, "5V I: %d", power_status.raw_5v_i/13);
                ESP_LOGV(TAG, "BAT I: %d", power_status.raw_bat_i/13);
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
