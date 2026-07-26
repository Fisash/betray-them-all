#ifndef COMMAND_INFO_H
#define COMMAND_INFO_H

#include <stdint.h>

#define CMD_MAX_SYNONYMS 6

enum command_type {
    CMD_EXIT,
    CMD_MOVE,
    CMD_EXPLORE,
    CMD_WAIT,
    CMD_INFO_UNIT,
    CMD_INFO_ITEM,
    CMD_EQUIP,
    CMD_UNEQUIP,
    CMD_IMPROVE,
    CMD_HELP,

    CMD_SHOP_BUY_ITEM,
    CMD_SHOP_SELL_ITEM,

    CMD_UNKNOWN,
    CMD_COUNT = CMD_UNKNOWN
};

struct command_info {
    char *cmd_synonyms[CMD_MAX_SYNONYMS];
    uint8_t min_argc;
    const char *help_message;
    int is_need_redraw_after_execution;
};

void commands_info_init(struct command_info commands[]);

#endif
