#ifndef CLI_H
#define CLI_H

#include <stdint.h>

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

#define INPUT_BUF_SIZE 256
#define MAX_AGRC 64

typedef struct {
    uint8_t argc;
    char *argv [MAX_AGRC];
} command;

void cli_run(game_state_t *game_state, const game_info_t *game_info);

#endif
