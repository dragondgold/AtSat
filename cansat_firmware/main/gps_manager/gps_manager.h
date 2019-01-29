#ifndef __GPS_MANAGER_H__
#define __GPS_MANAGER_H__

#include <stdbool.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "libs/minmea/minmea.h"

#define GPS_MANAGER_UART_BUFFER_SIZE    512

esp_err_t gps_manager_init(void);

bool gps_manager_is_valid_location(void);
struct minmea_sentence_gga gps_manager_get_gga(void);

#endif // __GPS_MANAGER_H__
