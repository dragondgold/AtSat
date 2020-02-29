#ifndef __BLE_MANAGER_H__
#define __BLE_MANAGER_H__

#include <stdbool.h>
#include <time.h>

#include "nordic_common.h"
#include "nrf.h"
#include "app_timer.h"
#include "ble_gap.h"

#define APP_BLE_OBSERVER_PRIO           3                                       // Application's BLE observer priority. You shouldn't need to modify this value.
#define APP_BLE_CONN_CFG_TAG            1                                       // A tag identifying the SoftDevice BLE configuration.
#define DEVICE_NAME                     "BeeCounter"                            // Name of device. Will be included in the advertising data.

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(500, UNIT_1_25_MS)        // Minimum acceptable connection interval (0.5 seconds).
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(4000, UNIT_1_25_MS)       // Maximum acceptable connection interval (4 second).
#define SLAVE_LATENCY                   0                                       // Slave latency.
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(32000, UNIT_10_MS)        // Connection supervisory time-out (32 seconds).

#define APP_ADV_INTERVAL                MSEC_TO_UNITS(5000, UNIT_0_625_MS)      // The advertising interval (5 seconds)
#define APP_ADV_DURATION                BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED   // The advertising time-out (in units of seconds). When set to 0, we will never time out.

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(10000)                  // Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds).
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                  // Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds).
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                       // Number of attempts before giving up the connection parameter negotiation.

bool ble_manager_init();

void ble_manager_advertising_start();

void ble_manager_update_battery(uint8_t battery_soc);
void ble_manager_update_time(time_t timestamp);
void ble_manager_update_temperature(float temperature);
void ble_manager_update_bee_counter(uint32_t input_count, uint32_t output_count, uint32_t input_flow, uint32_t output_flow, uint8_t temperature, time_t timestamp);

#endif