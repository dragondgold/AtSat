#ifndef __COM_MANAGER_H__
#define __COM_MANAGER_H__

#include "esp_err.h"

#define COM_MANAGER_CARRIER_FREQUENCY   915.0

esp_err_t com_manager_init(void);

#endif // __COM_MANAGER_H__
