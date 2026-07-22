#ifndef CLI_BATTLE_H
#define CLI_BATTLE_H

#include "core/state/battle_state.h"
#include "core/defs/game_info.h"

void cli_battle_run(struct battle_state *battle, const struct game_info *info);

#endif
