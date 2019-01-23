#ifndef __AUX_PS_H__
#define __AUX_PS_H__

#include <stdbool.h>
#include "esp_system.h"

esp_err_t aux_ps_init(void);

void aux_ps_enable(void);
void aux_ps_disable(void);
bool aux_ps_is_enabled(void);

#endif