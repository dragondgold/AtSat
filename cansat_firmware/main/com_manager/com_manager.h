#ifndef __COM_MANAGER_H__
#define __COM_MANAGER_H__

#include "esp_err.h"
#include "libs/axtec_packet/axtec_packet.h"
#include <stdbool.h>

typedef struct
{
    bool overcurrent_3v3;
    bool overcurrent_5v;
    bool overcurrent_bat;
    bool overvoltage_3v3;
    bool overvoltage_5v;
} error_flags_t;

#define COM_MANAGER_CARRIER_FREQUENCY           915.0
#define COM_MANAGER_QUEUE_SIZE                  20
#define COM_MANAGER_QUEUE_ELEMENT_SIZE          sizeof(axtec_encoded_packet_t)
#define COM_MANAGER_DEFAULT_REPORT_PERIOD       200     // 200 ms
#define COM_MANAGER_DEFAULT_REPORTS_ENABLED     false
#define COM_MANAGER_MAX_SENSOR_IDS              20
#define COM_MANAGER_AUTO_CLOSE_TIME             10000   // Automatic close time for parachute and balloon in ms

esp_err_t com_manager_init(void);

void com_manager_add_packet(axtec_decoded_packet_t* packet);

#endif // __COM_MANAGER_H__
