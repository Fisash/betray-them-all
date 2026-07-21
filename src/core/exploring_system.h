#ifndef EXPLORING_SYSTEM_H
#define EXPLORING_SYSTEM_H

#include "core/state/world.h"
#include "core/state/squad.h"
#include "core/defs/events_info.h"

void exploring_system_explore_cell(struct cell* cell, struct cell **cd_list,
                        const struct events_info *info, int16_t *active,
                                   const struct cell_info cells_info[]);

void exploring_system_explore_squad_cell(struct squad *squad, struct world *world,
                              const struct events_info *info, int16_t *active,
                                         const struct cell_info cells_info[]);
#endif
