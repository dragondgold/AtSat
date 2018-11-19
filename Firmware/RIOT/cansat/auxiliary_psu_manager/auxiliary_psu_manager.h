#ifndef AUXILIARY_PSU_MANAGER_H_
#define AUXILIARY_PSU_MANAGER_H_

#include <stdbool.h>

void auxiliary_psu_manager_init(void);

void auxiliary_psu_manager_turn_on(void);
void auxiliary_psu_manager_turn_off(void);

bool auxiliary_psu_manager_is_on(void);

void auxiliary_psu_manager_acquire(void);
void auxiliary_psu_manager_release(void);

#endif