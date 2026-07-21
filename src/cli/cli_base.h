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

struct command cli_base_input_command(char input_buffer[]);

int cli_base_choose_number(uint8_t min, uint8_t max);

void print_item_info(const struct item *item, const struct item_info info[],
                                                    struct unit *unit);

void print_stat_with_scaling(const char *label, int base_value,
               int real_value, const struct scaling_group *scalings,
                                             int is_show_real);
#endif
