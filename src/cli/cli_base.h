#ifndef CLI_BASE_H
#define CLI_BASE_H

#include <stdint.h>
#include "core/state/game_state.h"

#define INPUT_BUF_SIZE 256
#define MAX_AGRC 16

#define ARGS_SPLITER " \n"

typedef struct {
    uint8_t argc;
    char *argv [MAX_AGRC];
} command_t;

command_t cli_base_input_command(char input_buffer[]);

int cli_base_choose_number(uint8_t min, uint8_t max);

void print_item_info(const item_t *item, const item_info_t info[],
                                                    unit_t *unit);

void print_stat_with_scaling(const char *label, int base_value,
               int real_value, const scaling_group_t *scalings,
                                             int is_show_real);
#endif
