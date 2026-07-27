#ifndef COMMAND_INFO_H
#define COMMAND_INFO_H

#include <stdint.h>

#define CMD_MAX_SYNONYMS 6

enum command_type {
    CMD_NEWGAME,
    CMD_LOADGAME,
    CMD_SETTINGS,

    CMD_EXIT,
    CMD_MOVE,
    CMD_EXPLORE,
    CMD_WAIT,

    CMD_INFO_WORLD,
    CMD_SQUAD_INFO_BASE,
    CMD_SQUAD_INFO_UNIT,
    CMD_SQUAD_INFO_UNITS,
    CMD_SQUAD_INFO_ITEM,
    CMD_SQUAD_INFO_INV,

    CMD_EQUIP,
    CMD_UNEQUIP,
    CMD_IMPROVE,

    CMD_SHOP_BUY,
    CMD_SHOP_SELL,
    CMD_SHOP_INFO_ITEM,
    CMD_SHOP_INFO_INV,

    CMD_HELP,

    CMD_UNKNOWN,
    CMD_COUNT = CMD_UNKNOWN
};

struct command_info {
    const char *synonyms[CMD_MAX_SYNONYMS];
    const uint8_t min_argc;
    const char *help_message;
};

void commands_info_init(const struct command_info *commands[]);

#endif
