#include "console_manager.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "config/cansat.h"
#include "libs/minmea/minmea.h"
#include "sensor_manager/sensor_manager.h"
#include "servo_manager/servo_manager.h"
#include "aux_ps/aux_ps.h"

#include "esp_log.h"

static const char* TAG = "console";

// Task
static StackType_t stack[CONSOLE_MANAGER_STACK_SIZE];
static StaticTask_t task;
static TaskHandle_t task_handle;

static int free_mem_cmd(int argc, char** argv)
{
    printf("%d bytes\n", esp_get_free_heap_size());
    return 0;
}

static int samples_cmd(int argc, char** argv)
{
    sensors_data_t* samples = sensor_manager_get_samples();

    for(unsigned int n = 0; n < SENSOR_MANAGER_MAX_SAMPLES_TO_STORE; ++n)
    {
        if(samples[n].valid)
        {
            printf("Sample %d: %d, %.2f %.2f %.2f, %.2f %.2f %.2f, %.2f %.2f %.2f, %d, %d, %d, %.5f %.5f, %.2f\n", 
                n,
                (uint32_t)samples[n].timestamp,
                samples[n].gyro.x, samples[n].gyro.y, samples[n].gyro.z,
                samples[n].acc.x, samples[n].acc.y, samples[n].acc.z,
                samples[n].mag.x, samples[n].mag.y, samples[n].mag.z,
                (unsigned int)samples[n].pressure,
                (unsigned int)samples[n].temperature,
                (unsigned int)samples[n].humidity,
                minmea_tocoord(&samples[n].latitude),
                minmea_tocoord(&samples[n].longitude),
                samples[n].altitude);
        }
    }

    return 0;
}

static int open_servo(int argc, char** argv)
{
    servo_manager_open_balloon();
    servo_manager_open_parachute();
    printf("\n");
    return 0;
}

static int close_servo(int argc, char** argv)
{
    servo_manager_close_balloon();
    servo_manager_close_parachute();
    printf("\n");
    return 0;
}

static int ps_cmd(int argc, char** argv)
{
    aux_ps_enable();
    printf("\n");
    return 0;
}

void console_task(void* arg)
{
    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    const char* prompt = LOG_COLOR_I "esp32> " LOG_RESET_COLOR;

    // Figure out if the terminal supports escape sequences
    int probe_status = linenoiseProbe();
    
    // Zero indicates success
    if (probe_status) 
    {
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);

        // Since the terminal doesn't support escape sequences,
        //  don't use color codes in the prompt.
        prompt = "esp32> ";
    }

    while(true)
    {
        // Get a line using linenoise.
        // The line is returned when ENTER is pressed.
        char* line = linenoise(prompt);

        // Ignore empty lines
        if (line != NULL) {
            // Add the command to the history
            linenoiseHistoryAdd(line);

            // Try to run the command
            int ret;
            esp_err_t err = esp_console_run(line, &ret);

            if (err == ESP_ERR_NOT_FOUND) 
            {
                printf("Unrecognized command\n");
            } 
            else if (err == ESP_ERR_INVALID_ARG) {
                // command was empty
            }
            else if (err == ESP_OK && ret != ESP_OK) 
            {
                printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(err));
            }
            else if (err != ESP_OK) 
            {
                printf("Internal error: %s\n", esp_err_to_name(err));
            }

            // linenoise allocates line buffer on the heap, so need to free it
            linenoiseFree(line);
        }
    }
}

esp_err_t console_manager_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing");

    // Disable buffering on stdin and stdout
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Minicom, screen, idf_monitor send CR when ENTER key is pressed
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    // Move the caret to the beginning of the next line on '\n'
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    // Install UART driver for interrupt-driven reads and writes
    if((err = uart_driver_install(UART_NUM_0, 256, 0, 8, NULL, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error installing UART driver: %d", err);
        return err;
    }

    // Tell VFS to use UART driver
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    /* Initialize the console */
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
            .hint_color = atoi(LOG_COLOR_CYAN)
    };
    ESP_ERROR_CHECK(esp_console_init(&console_config));

    // Configure linenoise line completion library
    // Enable multiline editing. If not set, long commands will scroll within single line.
    linenoiseSetMultiLine(1);

    // Tell linenoise where to get command completions and hints
    linenoiseSetCompletionCallback(&esp_console_get_completion);
    linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);

    // Set command history size
    linenoiseHistorySetMaxLen(100);

    // Register commands
    const esp_console_cmd_t cmd_free = {
        .command = "free",
        .help = "Get the total size of heap memory available",
        .hint = NULL,
        .func = &free_mem_cmd,
    };
    const esp_console_cmd_t cmd_samples = {
        .command = "samples",
        .help = "Get all samples stored",
        .hint = NULL,
        .func = &samples_cmd,
    };
    const esp_console_cmd_t cmd_open = {
        .command = "open",
        .help = "Open servo",
        .hint = NULL,
        .func = &open_servo,
    };
    const esp_console_cmd_t cmd_close = {
        .command = "close",
        .help = "Close servo",
        .hint = NULL,
        .func = &close_servo,
    };
    const esp_console_cmd_t cmd_ps = {
        .command = "ps",
        .help = "Power supply control",
        .hint = NULL,
        .func = &ps_cmd,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_free));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_samples));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_open));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_close));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_ps));

    task_handle = xTaskCreateStaticPinnedToCore(console_task, "console", CONSOLE_MANAGER_STACK_SIZE, 
        NULL, CONSOLE_MANAGER_TASK_PRIORITY, stack, &task, CONSOLE_MANAGER_AFFINITY);

    ESP_LOGI(TAG, "Ready!");

    return ESP_OK;
}
