#ifndef CORE_H
#define CORE_H

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

struct core {
    struct game_state *state;
    const struct game_info *info;
};

void core_init(struct core *core);

void core_new_session(struct core *core);

#endif
