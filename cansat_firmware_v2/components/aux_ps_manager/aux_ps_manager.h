#ifndef __AUX_PS_H__
#define __AUX_PS_H__

#include <stdbool.h>
#include "nrf_gpio.h"

#define AUX_PS_ENABLE_PIN               NRF_GPIO_PIN_MAP(0, 14)
#define AUX_PS_MUTEX_TIMEOUT            (200 / portTICK_PERIOD_MS)

bool aux_ps_manager_init(void);

void aux_ps_manager_enable(void);
void aux_ps_manager_disable(void);
bool aux_ps_manager_is_enabled(void);

#endif