#ifndef CLI_BASE_H
#define CLI_BASE_H

#include <stdint.h>
#include "core/state/game_state.h"

#define INPUT_BUF_SIZE 256
#define MAX_AGRC 16

#define ARGS_SPLITER " \n"

struct command {
    uint8_t argc;
    char *argv [MAX_AGRC];
};

void cli_base_input_command(struct command *cmd, char input_buffer[]);

int cli_base_choose_number(uint8_t min, uint8_t max);

#endif
