#ifndef GAME_H
#define GAME_H

#include "core/state/game_state.h"
#include "core/defs/game_info.h"
#include "cli/command_inputer.h"

struct game {
    struct game_state *state;
    const struct game_info *info;
    struct command_inputer *inputer;
    int is_running;
};

void game_init(struct game *game);
void game_run(struct game *game);

#endif
