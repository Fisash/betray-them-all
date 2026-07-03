#ifndef EXPLORING_SYSTEM_H
#define EXPLORING_SYSTEM_H

#include "core/state/world.h"
#include "core/state/squad.h"
#include "core/defs/events_info.h"

#define EXPLORE_CELL_COOLDOWN 15

void exploring_system_explore_cell(cell_t* cell, cell_t **cd_list,
                       const events_info_t *info, int16_t *active);

void exploring_system_explore_squad_cell(squad_t *squad, world_t *world,
                             const events_info_t *info, int16_t *active);
#endif
