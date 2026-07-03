#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

#include "core/state/squad.h"
#include "core/state/world.h"

void time_system_spend(squad_t *squad, world_t *world, uint16_t days, 
                                                uint16_t *days_state);
  
#endif
