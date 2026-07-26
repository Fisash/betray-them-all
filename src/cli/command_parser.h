#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

#include <stdint.h>

#include "cli/command_info.h"

#include "core/defs/items_info.h"
#include "core/defs/unit_stats.h"
#include "core/state/squad.h"

#define ARGC_MAX 8

struct command {
    uint8_t argc;
    char *argv [ARGC_MAX];
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
    const struct command_info *info;
    int is_correct;
    union {
        enum move_order move_order;
        uint16_t unit_num;
        uint16_t item_num;
        struct cmd_args_equip equip;
        struct cmd_args_unequip unequip;
        struct cmd_args_improve improve;
        uint16_t shop_item_num;
    } args;
};

void command_parse_lexical(char *input_buf, struct command *out);

void command_parse_syntax(struct parsed_command *out, 
                          const struct command *lex_cmd,
                          const struct command_info *info[]);
#endif
