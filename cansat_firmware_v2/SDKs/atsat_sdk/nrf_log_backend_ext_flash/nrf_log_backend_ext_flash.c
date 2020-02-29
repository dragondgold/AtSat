#include "sdk_common.h"
#if NRF_MODULE_ENABLED(NRF_LOG)
#include "ext_flash_manager.h"
#include "nrf_log_backend_ext_flash_config.h"
#include "nrf_log_backend_ext_flash.h"
#include "nrf_log_backend_serial.h"
#include "nrf_log_str_formatter.h"
#include "nrf_log_internal.h"
#include "nrf_delay.h"
#include <stdint.h>

static uint8_t m_string_buff[NRF_LOG_BACKEND_EXT_FLASH_BUFFER_SIZE];

void nrf_log_backend_ext_flash_init(void)
{
    // 'ext_flash_manager' should already be initialized. Initialize the log file.
    ext_flash_manager_start_log();
}

static void write_to_flash(void const * p_context, char const * buffer, size_t len)
{
    (void)p_context;

    if (len) {
        ext_flash_manager_write_log(buffer, len);
    }
}
static void nrf_log_backend_ext_flash_put(nrf_log_backend_t const * p_backend,
                               nrf_log_entry_t * p_msg)
{
    (void)p_backend;
    nrf_log_backend_serial_put(p_backend, p_msg, m_string_buff, NRF_LOG_BACKEND_EXT_FLASH_BUFFER_SIZE, write_to_flash);
}

static void nrf_log_backend_ext_flash_flush(nrf_log_backend_t const * p_backend)
{
    (void)p_backend;
    ext_flash_manager_sync_log();
}

static void nrf_log_backend_ext_flash_panic_set(nrf_log_backend_t const * p_backend)
{
    (void)p_backend;
    ext_flash_manager_close_log();
}

const nrf_log_backend_api_t nrf_log_backend_ext_flash_api = {
    .put       = nrf_log_backend_ext_flash_put,
    .flush     = nrf_log_backend_ext_flash_flush,
    .panic_set = nrf_log_backend_ext_flash_panic_set
};
#endif
