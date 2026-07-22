#ifndef ENTRY_H
#define ENTRY_H

#include <stdint.h>

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

void cli_run(struct game_state *game_state, const struct game_info *game_info);

#endif
