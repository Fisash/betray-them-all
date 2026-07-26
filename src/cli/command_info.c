#include <stdlib.h>
#include <string.h>

#include "cli/command_info.h"

static const struct command_info cmd_exit = {
    { "exit", "quit", "leave", "q", NULL},
    0, 
    NULL
};

static const struct command_info cmd_move = {
    { "move", "mov", "m", NULL},
    1, 
    "mov [r|l|u|d|s]"
};

static const struct command_info cmd_explore = {
    { "explore", "e", NULL},
    0, 
    NULL
};

static const struct command_info cmd_wait = {
    { "wait", "skip", NULL},
    0, 
    NULL
};

static const struct command_info cmd_info_world = {
    { "world", "map", NULL},
    0, 
    NULL
};

static const struct command_info cmd_info_base = {
    { "info", "base", NULL},
    0, 
    NULL
};

static const struct command_info cmd_info_unit = {
    { "unit", NULL},
    1, 
    "unit [N unit]"
};

static const struct command_info cmd_info_units = {
    { "units", NULL},
    0, 
    NULL
};

static const struct command_info cmd_info_item = {
    { "item", NULL},
    1, 
    "item [N item]"
};

static const struct command_info cmd_info_inv = {
    { "inv", "inventory", "items", NULL},
    0, 
    NULL
};

static const struct command_info cmd_equip = {
    { "equip", NULL},
    2, 
    "equip [N unit] [N item]"
};

static const struct command_info cmd_unequip = {
    { "unequip", NULL},
    2, 
    "unequip [N unit] [weapon|armor]"
};

static const struct command_info cmd_improve = {
    { "improve", "imp", NULL},
    2, 
    "improve [N unit] [str|agl|wil|int]"
};

static const struct command_info cmd_shop_buy = {
    { "buy", NULL},
    1, 
    "buy [N seller`s item]"
};

static const struct command_info cmd_shop_sell = {
    { "sell", NULL},
    1, 
    "sell [N squad`s item]"
};

static const struct command_info cmd_shop_info_item = {
    { "shop_item", NULL},
    1, 
    "shop item [N seller`s item]"
};

static const struct command_info cmd_shop_info_inv = {
    { "shop_inv", "shop_inventory", "shop_items", NULL},
    0, 
    NULL
};

static const struct command_info cmd_help = {
    { "help", "?", NULL},
    0, 
    NULL
};


void commands_info_init(const struct command_info *commands[])
{
    commands[CMD_EXIT] = &cmd_exit; 
    commands[CMD_MOVE] = &cmd_move;
    commands[CMD_EXPLORE] = &cmd_explore; 
    commands[CMD_WAIT] = &cmd_wait; 

    commands[CMD_INFO_WORLD] = &cmd_info_world; 
    commands[CMD_SQUAD_INFO_BASE] = &cmd_info_base;
    commands[CMD_SQUAD_INFO_UNIT] = &cmd_info_unit;
    commands[CMD_SQUAD_INFO_UNITS] = &cmd_info_units;
    commands[CMD_SQUAD_INFO_ITEM] = &cmd_info_item;
    commands[CMD_SQUAD_INFO_INV] = &cmd_info_inv;

    commands[CMD_EQUIP] = &cmd_equip;
    commands[CMD_UNEQUIP] = &cmd_unequip;
    commands[CMD_IMPROVE] = &cmd_improve;

    commands[CMD_SHOP_BUY] = &cmd_shop_buy;
    commands[CMD_SHOP_SELL] = &cmd_shop_sell;
    commands[CMD_SHOP_INFO_ITEM] = &cmd_shop_info_item;
    commands[CMD_SHOP_INFO_INV] = &cmd_shop_info_inv;
    commands[CMD_HELP] = &cmd_help;
}
