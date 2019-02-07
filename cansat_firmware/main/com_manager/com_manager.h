#ifndef __COM_MANAGER_H__
#define __COM_MANAGER_H__

#include "esp_err.h"
#include "libs/axtec_packet/axtec_packet.h"
#include <stdbool.h>

#define COM_MANAGER_CARRIER_FREQUENCY       915.0
#define COM_MANAGER_QUEUE_SIZE              20
#define COM_MANAGER_QUEUE_ELEMENT_SIZE      sizeof(axtec_encoded_packet_t)
#define COM_MANAGER_DEFAULT_REPORT_PERIOD   1000    // 1000 ms
#define COM_MANAGER_DEFAULT_REPORTS_ENABLED true

esp_err_t com_manager_init(void);

#endif // __COM_MANAGER_H__
