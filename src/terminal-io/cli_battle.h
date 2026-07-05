#ifndef CLI_BATTLE_H
#define CLI_BATTLE_H

#include "core/state/battle_state.h"
#include "core/defs/game_info.h"

int cli_choose_digit(uint8_t min, uint8_t max);

void cli_battle_run(battle_state_t *battle, const game_info_t *info);

#endif
