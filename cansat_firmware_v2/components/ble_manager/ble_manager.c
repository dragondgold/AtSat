#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "ble_manager.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdm.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"

// BLE services
#include "ble_tms.h"
#include "ble_bas.h"
#include "ble_dis.h"

#include "app_config.h"
#include "app_button.h"
#include "app_error.h"
#include "boards.h"

#define NRF_LOG_MODULE_NAME ble_manager
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

// Universally unique service identifiers.
static ble_uuid_t m_adv_uuids[] =
{
    {BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE},
    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE},
    {BLE_UUID_TIME_SERVICE, BLE_UUID_TYPE_BLE}
};

BLE_ADVERTISING_DEF(m_advertising);                                             // Advertising module instance.
NRF_BLE_GATT_DEF(m_gatt);                                                       // GATT module instance.
NRF_BLE_QWR_DEF(m_qwr);                                                         // Context for the Queued Write module.
BLE_BAS_DEF(battery_service);                                                   // Battery service instance
BLE_TMS_DEF(m_tms);                                                             // Time service instance

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                        // Handle of the current connection.

/**
 * @brief Function for starting advertising.
 */
static void advertising_start(void)
{
    ret_code_t           err_code;

    err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);

    //bsp_board_led_on(ADVERTISING_LED);
}

char const * phy_str(ble_gap_phys_t phys)
{
    static char const * str[] =
    {
        "1 Mbps",
        "2 Mbps",
        "Coded",
        "Unknown"
    };

    switch (phys.tx_phys)
    {
        case BLE_GAP_PHY_1MBPS:
            return str[0];

        case BLE_GAP_PHY_2MBPS:
        case BLE_GAP_PHY_2MBPS | BLE_GAP_PHY_1MBPS:
        case BLE_GAP_PHY_2MBPS | BLE_GAP_PHY_1MBPS | BLE_GAP_PHY_CODED:
            return str[1];

        case BLE_GAP_PHY_CODED:
            return str[2];

        default:
            return str[3];
    }
}

/**
 * @brief Function for handling BLE events.
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    uint32_t err_code;
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected");
            //err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            //APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            NRF_LOG_INFO("Connection interval updated: 0x%x, 0x%x.",
                p_gap_evt->params.conn_param_update.conn_params.min_conn_interval,
                p_gap_evt->params.conn_param_update.conn_params.max_conn_interval);
        } break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            // Accept parameters requested by the peer.
            ble_gap_conn_params_t params;
            params = p_gap_evt->params.conn_param_update_request.conn_params;
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle, &params);
            APP_ERROR_CHECK(err_code);

            NRF_LOG_INFO("Connection interval updated (upon request): 0x%x, 0x%x.",
                p_gap_evt->params.conn_param_update_request.conn_params.min_conn_interval,
                p_gap_evt->params.conn_param_update_request.conn_params.max_conn_interval);
        } 
        break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
        {
            err_code = sd_ble_gatts_sys_attr_set(p_gap_evt->conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
        } 
        break;

        case BLE_GATTC_EVT_TIMEOUT: // Fall through.
        case BLE_GATTS_EVT_TIMEOUT:
        {
            NRF_LOG_DEBUG("GATT timeout, disconnecting.");
            err_code = sd_ble_gap_disconnect(m_conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        } 
        break;

        case BLE_GAP_EVT_PHY_UPDATE:
        {
            ble_gap_evt_phy_update_t const * p_phy_evt = &p_ble_evt->evt.gap_evt.params.phy_update;

            if (p_phy_evt->status == BLE_HCI_STATUS_CODE_LMP_ERROR_TRANSACTION_COLLISION)
            {
                // Ignore LL collisions.
                NRF_LOG_DEBUG("LL transaction collision during PHY update.");
                break;
            }

            ble_gap_phys_t phys = {0};
            phys.tx_phys = p_phy_evt->tx_phy;
            phys.rx_phys = p_phy_evt->rx_phy;
            NRF_LOG_INFO("PHY update %s. PHY set to %s.",
                         (p_phy_evt->status == BLE_HCI_STATUS_CODE_SUCCESS) ?
                         "accepted" : "rejected",
                         phy_str(phys));
        } 
        break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } 
        break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            NRF_LOG_INFO("BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST");
            break;

        default:
            // No implementation needed.
            break;
    }
}

/**
 * @brief Function for the GAP initialization.
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    NRF_LOG_INFO("Initializing BLE GAP");
    ret_code_t              err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    NRF_LOG_INFO("Initializing GATT module");
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for handling Queued Write Module errors.
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static void battery_service_event_handler(ble_bas_t * p_bas, ble_bas_evt_t * p_evt)
{
    
}

static void time_service_event_handler(ble_tms_t * p_tms, ble_tms_evt_t * p_evt)
{

}

/**
 * @brief Function for initializing the BLE services that will be used by the application.
 */
static void services_init(void)
{
    ret_code_t          err_code;
    nrf_ble_qwr_init_t  qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize battery service
    NRF_LOG_INFO("Initializing battery service");
    const ble_bas_init_t bas_init = {
        .evt_handler = battery_service_event_handler,
        .support_notification = true,
        .p_report_ref = NULL,
        .initial_batt_level = 22,
        .bl_rd_sec = SEC_OPEN,
        .bl_cccd_wr_sec = SEC_OPEN,
        .bl_report_rd_sec = SEC_OPEN
    };
    err_code = ble_bas_init(&battery_service, &bas_init);
    APP_ERROR_CHECK(err_code);

    // Initialize device information service
    NRF_LOG_INFO("Initializing device information service");
    ble_dis_init_t dis_init = {0};
    ble_dis_sys_id_t sys_id = {
        .manufacturer_id = MANUFACTURER_ID,
        .organizationally_unique_id = ORG_UNIQUE_ID
    };

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, MANUFACTURER_NAME);
    ble_srv_ascii_to_utf8(&dis_init.model_num_str, MODEL_NUM);
    ble_srv_ascii_to_utf8(&dis_init.serial_num_str, SERIAL_NUMBER);
    ble_srv_ascii_to_utf8(&dis_init.hw_rev_str, HW_VERSION);
    ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, FW_VERSION);
    dis_init.p_sys_id = &sys_id;
    dis_init.dis_char_rd_sec = SEC_OPEN;
    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);

    // Initialize the time service
    NRF_LOG_INFO("Initializing time service");
    const ble_tms_init_t tms_init = {
        .evt_handler = time_service_event_handler,
        .bl_rd_sec = SEC_OPEN,
        .bl_wr_sec = SEC_OPEN,
        .bl_cccd_wr_sec = SEC_OPEN,
        .bl_report_rd_sec = SEC_OPEN
    };
    err_code = ble_tms_init(&m_tms, &tms_init);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for handling advertising events.
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising.");
            //err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            //APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
            //sleep_mode_enter();
            break;

        default:
            break;
    }
}

/** 
 * @brief Function for initializing the Advertising functionality.
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void advertising_init(void)
{
    NRF_LOG_INFO("Initializing BLE advertising module");
    ret_code_t             err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance      = true;
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;

    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**
 * @brief Function for handling the Connection Parameters Module.
 * @details This function will be called for all events in the Connection Parameters Module that
 *          are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply
 *       setting the disconnect_on_fail config parameter, but instead we use the event
 *       handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**
 * @brief Function for handling a Connection Parameters error.
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**
 * @brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    NRF_LOG_INFO("Initializing connection parameters service");
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

bool ble_manager_init() 
{
    ret_code_t err_code;

    NRF_LOG_INFO("Starting BLE stack");
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);

    gap_params_init();
    gatt_init();
    NRF_LOG_FLUSH();

    advertising_init();
    services_init();
    NRF_LOG_FLUSH();

    conn_params_init();

    return true;
}

void ble_manager_advertising_start() 
{
    advertising_start();
}

void ble_manager_update_battery(uint8_t battery_soc)
{
    ret_code_t err_code = ble_bas_battery_level_update(&battery_service, battery_soc, BLE_CONN_HANDLE_ALL);
    if ((err_code != NRF_SUCCESS) &&
        (err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != NRF_ERROR_RESOURCES) &&
        (err_code != NRF_ERROR_BUSY) &&
        (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
       )
    {
        NRF_LOG_ERROR("Error updating battery level: %d", err_code);
    }
}

void ble_manager_update_time(time_t timestamp)
{
    ret_code_t err_code = ble_tms_time_update(&m_tms, timestamp, BLE_CONN_HANDLE_ALL);
    if ((err_code != NRF_SUCCESS) &&
        (err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != NRF_ERROR_RESOURCES) &&
        (err_code != NRF_ERROR_BUSY) &&
        (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
       )
    {
        NRF_LOG_ERROR("Error updating timestamp: %d", err_code);
    }
}

void ble_manager_update_temperature(float temperature)
{
    
}