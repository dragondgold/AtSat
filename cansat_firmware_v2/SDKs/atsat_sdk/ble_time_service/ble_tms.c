#include <string.h>
#include <time.h>

#include "sdk_common.h"
#include "ble_tms.h"
#include "ble_srv_common.h"
#include "ble_conn_state.h"

#define NRF_LOG_MODULE_NAME ble_tms
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/**
 * @brief Function for handling the Write event.
 * @param[in]   p_tms       Time service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_tms_t * p_tms, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    // Check if notifications are enabled/disabled
    if (p_evt_write->handle == p_tms->timestamp_handle.cccd_handle && p_evt_write->len == 2)
    {
        if (p_tms->evt_handler == NULL)
        {
            return;
        }

        ble_tms_evt_t evt;

        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            evt.evt_type = BLE_TMS_EVT_NOTIFICATION_ENABLED;
        }
        else
        {
            evt.evt_type = BLE_TMS_EVT_NOTIFICATION_DISABLED;
        }
        evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;

        // CCCD written, call application event handler.
        p_tms->evt_handler(p_tms, &evt);
    }

    // Check if the value from the timestamp characteristic was written
    if (p_evt_write->handle == p_tms->timestamp_handle.value_handle && p_evt_write->len == sizeof(time_t))
    {
        if (p_tms->evt_handler == NULL)
        {
            return;
        }

        ble_tms_evt_t evt = {
            .evt_type = BLE_TMS_EVT_TIMESTAMP_WRITTEN,
            .conn_handle = p_ble_evt->evt.gatts_evt.conn_handle
        };
        memcpy(&p_tms->last_timestamp, p_ble_evt->evt.gatts_evt.params.write.data, p_ble_evt->evt.gatts_evt.params.write.len);

        // Call application event handler.
        p_tms->evt_handler(p_tms, &evt);
    }
}


void ble_tms_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    if ((p_context == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    ble_tms_t * p_tms = (ble_tms_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_tms, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**
 * @brief Function for adding the service characteristics
 * @param[in]   p_tms        Time service structure.
 * @param[in]   p_tms_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static ret_code_t add_characteristic(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ret_code_t              err_code;
    ble_add_char_params_t   add_char_params;
    time_t                  initial_time = 0;

    // Add timestamp characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = BLE_TMS_UUID_INPUT_OUTPUT_COUNT_CHAR;
    add_char_params.max_len           = sizeof(time_t);
    add_char_params.init_len          = sizeof(time_t);
    add_char_params.p_init_value      = (uint8_t *)&initial_time;
    add_char_params.char_props.notify = true;
    add_char_params.char_props.read   = 1;
    add_char_params.char_props.write  = 1;
    add_char_params.cccd_write_access = p_tms_init->bl_cccd_wr_sec;
    add_char_params.read_access       = p_tms_init->bl_rd_sec;
    add_char_params.write_access      = p_tms_init->bl_wr_sec;

    err_code = characteristic_add(p_tms->service_handle,
                                  &add_char_params,
                                  &(p_tms->timestamp_handle));
    
    return err_code;
}

ret_code_t ble_tms_init(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    if (p_tms == NULL || p_tms_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    ret_code_t err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_tms->evt_handler      = p_tms_init->evt_handler;
    p_tms->last_timestamp   = 0;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_TIME_SERVICE);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_tms->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add characteristic
    err_code = add_characteristic(p_tms, p_tms_init);
    return err_code;
}


/**
 * @brief Function for sending notifications.
 * @param[in]   p_hvx_params Pointer to structure with notification data.
 * @param[in]   conn_handle  Connection handle.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static ret_code_t send_notification(ble_gatts_hvx_params_t * const p_hvx_params, uint16_t conn_handle)
{
    ret_code_t err_code = sd_ble_gatts_hvx(conn_handle, p_hvx_params);
    return err_code;
}


ret_code_t ble_tms_time_update(ble_tms_t * p_tms, time_t time, uint16_t conn_handle)
{
    if (p_tms == NULL)
    {
        return NRF_ERROR_NULL;
    }

    ret_code_t         err_code = NRF_SUCCESS;
    ble_gatts_value_t  gatts_value;

    if (time != p_tms->last_timestamp)
    {
        // Initialize value struct.
        memset(&gatts_value, 0, sizeof(gatts_value));

        gatts_value.len     = sizeof(time_t);
        gatts_value.offset  = 0;
        gatts_value.p_value = (uint8_t *)&time;

        // Update database.
        err_code = sd_ble_gatts_value_set(BLE_CONN_HANDLE_INVALID,
                                          p_tms->timestamp_handle.value_handle,
                                          &gatts_value);
        if (err_code == NRF_SUCCESS)
        {
            // Save new timestamp value.
            p_tms->last_timestamp = time;
        }
        else
        {
            return err_code;
        }

        // Send value if connected and notifying.
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_tms->timestamp_handle.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        if (conn_handle == BLE_CONN_HANDLE_ALL)
        {
            ble_conn_state_conn_handle_list_t conn_handles = ble_conn_state_conn_handles();

            // Try sending notifications to all valid connection handles.
            for (uint32_t i = 0; i < conn_handles.len; i++)
            {
                if (ble_conn_state_status(conn_handles.conn_handles[i]) == BLE_CONN_STATUS_CONNECTED)
                {
                    if (err_code == NRF_SUCCESS)
                    {
                        err_code = send_notification(&hvx_params, conn_handles.conn_handles[i]);
                    }
                    else
                    {
                        // Preserve the first non-zero error code
                        UNUSED_RETURN_VALUE(send_notification(&hvx_params, conn_handles.conn_handles[i]));
                    }
                }
            }
        }
        else
        {
            err_code = send_notification(&hvx_params, conn_handle);
        }
    }

    return err_code;
}
