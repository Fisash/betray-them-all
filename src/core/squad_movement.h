#ifndef SQUAD_MOVEMENT_H
#define SQUAD_MOVEMENT_H

#include "core/state/squad.h"

void movement_change_squad_pos_by_order(struct squad *squad);

void movement_try_move_squad_by_order(struct squad *squad);

void movement_try_move_squad(struct squad *squad, enum move_order order);

#endif
