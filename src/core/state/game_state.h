#ifndef GAME_H
#define GAME_H

#include "core/state/world.h"
#include "core/state/squad.h"
#include "core/state/battle_state.h"
#include "core/state/shop.h"

#include "core/defs/game_info.h"

typedef struct {
    world_t world;
    squad_t squad;
    battle_state_t battle;
    shop_t *active_shop;
    int16_t active_event_id;
    uint16_t days;
} game_state_t;

void game_state_init(game_state_t *game_state, const game_info_t *info);

#endif
