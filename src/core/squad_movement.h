#ifndef SQUAD_MOVEMENT_H
#define SQUAD_MOVEMENT_H

#include "core/state/squad.h"

void movement_change_squad_pos_by_order(squad_t *squad);

void movement_try_move_squad_by_order(squad_t *squad);

void movement_try_move_squad(squad_t *squad, move_order_t order);

#endif
