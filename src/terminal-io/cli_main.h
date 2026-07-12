#ifndef CLI_MAIN_H
#define CLI_MAIN_H

#include <stdint.h>

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

void cli_run(game_state_t *game_state, const game_info_t *game_info);

#endif
