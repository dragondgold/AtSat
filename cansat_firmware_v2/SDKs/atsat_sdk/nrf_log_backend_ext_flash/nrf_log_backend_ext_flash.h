#ifndef __NRF_LOG_BACKEND_EXT_FLASH_H__
#define __NRF_LOG_BACKEND_EXT_FLASH_H__

#include "nrf_log_backend_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const nrf_log_backend_api_t nrf_log_backend_ext_flash_api;

typedef struct {
    nrf_log_backend_t backend;
} nrf_log_backend_ext_flash_t;

/**
 * @brief RTT backend definition
 *
 * @param _name Name of the instance.
 */
#define NRF_LOG_BACKEND_EXT_FLASH_DEF(_name)  \
    NRF_LOG_BACKEND_DEF(_name, nrf_log_backend_ext_flash_api, NULL)

void nrf_log_backend_ext_flash_init(void);

#ifdef __cplusplus
}
#endif

#endif