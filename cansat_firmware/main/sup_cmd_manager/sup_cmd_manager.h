#ifndef __SUP_CMD_MANAGER_H__
#define __SUP_CMD_MANAGER_H__

#include "esp_system.h"

#define SUP_CMD_MANAGER_UART_BUFFER_SIZE    256

esp_err_t sup_cmd_manager_init(void);

#endif // __SUP_CMD_MANAGER_H__
