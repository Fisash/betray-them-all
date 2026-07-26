#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

#include <stdint.h>

#include "cli/lexical_parser.h"
#include "cli/command_info.h"

struct cmd_args_move { 
    enum move_order order; 
};

struct cmd_args_info_unit { 
    uint16_t unit_num;
};

struct cmd_args_info_item { 
    uint16_t item_num;
};

struct cmd_args_equip { 
    uint16_t unit_num;
    uint16_t item_num;
};

struct cmd_args_unequip { 
    uint16_t unit_num;
    enum item_type equipment_type;
};

struct cmd_args_improve { 
    uint16_t unit_num;
    enum stat_selection stat;
};

struct parsed_command {
    enum command_type type;
    int is_correct;
    union {
        struct cmd_args_move move;
        struct cmd_args_info_unit info_unit;
        struct cmd_args_info_item info_item;
        struct cmd_args_equip equip;
        struct cmd_args_unequip unequip;
        struct cmd_args_improve improve;
    } args;
};

void syntax_command_parse(struct parsed_command *out, 
                       const struct command *lex_cmd,
                   const struct command_info info[]);

void syntax_command_input(struct parsed_command *cmd, char input_buffer[],
                                        const struct command_info info[]);
#endif
