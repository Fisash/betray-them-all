#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "core/state/world.h"
#include "core/state/squad.h"
#include "core/state/battle_state.h"
#include "core/state/shop.h"

#include "core/defs/game_info.h"

enum game_status {
    LEAVE,
    LOSE,
    FIGHTING,
    SHOPPING,
    EVENT_HAPPENING,
    IDLE
};

struct game_state {
    struct world world;
    struct squad squad;
    struct battle_state battle;
    struct shop *active_shop;
    int16_t active_event_id;
    uint16_t days;

    uint8_t is_running;
    uint8_t is_over;
};

void game_state_init(struct game_state *game_state, const struct game_info *info);

enum game_status game_state_get_status(const struct game_state *state);

#endif
