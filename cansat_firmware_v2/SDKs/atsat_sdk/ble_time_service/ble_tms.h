#ifndef __BLE_TMS_H__
#define __BLE_TMS_H__

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_TMS_BLE_OBSERVER_PRIO               2
#define BLE_UUID_TIME_SERVICE                   0x1901
#define BLE_TMS_UUID_INPUT_OUTPUT_COUNT_CHAR    0x2AC9
#define BLE_TMS_UUID_INPUT_OUTPUT_FLOW_CHAR     0x2ACA

/**
 * @brief Macro for defining a ble_tms instance.
 * @param _name  Name of the instance.
 * @hideinitializer
 */
#define BLE_TMS_DEF(_name)                          \
    static ble_tms_t _name;                         \
    NRF_SDH_BLE_OBSERVER(_name ## _obs,             \
                         BLE_TMS_BLE_OBSERVER_PRIO, \
                         ble_tms_on_ble_evt,        \
                         &_name)

/**@brief Time service event type. */
typedef enum
{
    BLE_TMS_EVT_NOTIFICATION_ENABLED,   // Time service value notification enabled event.
    BLE_TMS_EVT_NOTIFICATION_DISABLED,  // Time service value notification disabled event.
    BLE_TMS_EVT_TIMESTAMP_WRITTEN
} ble_tms_evt_type_t;

/**@brief Time service event. */
typedef struct
{
    ble_tms_evt_type_t evt_type;        // Type of event.
    uint16_t           conn_handle;     // Connection handle
} ble_tms_evt_t;

// Forward declaration of the ble_tms_t type.
typedef struct ble_tms_s ble_tms_t;

/**@brief Time service event handler type. */
typedef void (* ble_tms_evt_handler_t) (ble_tms_t * p_tms, ble_tms_evt_t * p_evt);

/**@brief Time service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_tms_evt_handler_t  evt_handler;                    // Event handler to be called for handling events in the time service.
    security_req_t         bl_rd_sec;                      // Security requirement for reading the BL characteristic value.
    security_req_t         bl_wr_sec;                      // Security requirement for writing the BL characteristic value.
    security_req_t         bl_cccd_wr_sec;                 // Security requirement for writing the BL characteristic CCCD.
    security_req_t         bl_report_rd_sec;               // Security requirement for reading the BL characteristic descriptor.
} ble_tms_init_t;

/**@brief Time service structure. This contains various status information for the service. */
struct ble_tms_s
{
    ble_tms_evt_handler_t    evt_handler;               // Event handler to be called for handling events in the bee counter service
    uint16_t                 service_handle;            // Handle of timer service (as provided by the BLE stack).
    ble_gatts_char_handles_t timestamp_handle;          // Handle related to the timestamp characteristic.
    time_t                   last_timestamp;            // Last timestamp passed to the service
};


/**
 * @brief Function for initializing the time service.
 * @param[out]  p_tms       Time service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_tms_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
ret_code_t ble_tms_init(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init);


/**
 * @brief Function for handling the Application's BLE Stack events.
 * @details Handles all events from the BLE stack of interest to the time service.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 * @param[in]   p_context   Bee counter service structure.
 */
void ble_tms_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**
 * @brief Function for updating the service time.
 * @details The application calls this function at max once every second to update the timestamp.
 *          The timestamp characteristic will only be sent to the clients which have
 *          enabled notifications. \ref BLE_CONN_HANDLE_ALL can be used as a connection handle
 *          to send notifications to all connected devices.
 *
 * @param[in]   p_tms           Time service structure
 * @param[in]   time            Unix timestamp to set (seconds elapsed since 00:00 hours, Jan 1, 1970 UTC)
 * @param[in]   conn_handle     Connection handle
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
ret_code_t ble_tms_time_update(ble_tms_t * p_tms, time_t time, uint16_t conn_handle);

#ifdef __cplusplus
}
#endif

#endif
