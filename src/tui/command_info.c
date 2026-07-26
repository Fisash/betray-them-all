#include <stdlib.h>
#include <string.h>

#include "cli/command_info.h"

static const char 
    *cmd_synonyms_exit[] = { "exit", "quit", "q", NULL };

static const char 
    *cmd_synonyms_move[] = { "move", "mov", "m", NULL };
static const char 
    help_message_move[] = "mov [r|l|u|d|s]";

static const char 
    *cmd_synonyms_explore[] = { "explore", "e", NULL };

static const char 
    *cmd_synonyms_wait[] = { "wait", NULL };

static const char 
    *cmd_synonyms_info_unit[] = { "info unit", NULL };
static const char 
    help_message_info_unit[] = "info unit [N unit]";

static const char 
    *cmd_synonyms_info_item[] = { "info item", NULL };
static const char 
    help_message_info_item[] = "info item [N item]";

static const char 
    *cmd_synonyms_equip[] = { "equip", NULL };
static const char 
    help_message_equip[] = "equip [N unit] [N item]";

static const char 
    *cmd_synonyms_unequip[] = { "unequip", NULL };
static const char 
    help_message_unequip[] = "unequip [N unit] [weapon|armor]";

static const char 
    *cmd_synonyms_improve[] = { "improve", NULL };
static const char 
    help_message_improve[] = "improve [N unit] [str|agl|wil|int]";

static const char 
    *cmd_synonyms_buy[] = { "buy", NULL };
static const char 
    help_message_buy[] = "buy [N merchant item]";

static const char 
    *cmd_synonyms_sell[] = { "sell", NULL };
static const char 
    help_message_sell[] = "sell [N squad item]";

static const char 
    *cmd_synonyms_help[] = { "help", "?", NULL };

static void fill_synonyms(char *synonyms[], const char *strings[])
{
    int i = 0;
    while(strings[i] != NULL && i < CMD_MAX_SYNONYMS)
    {
        synonyms[i] = strings[i];
        i++;
    }
}

void commands_info_init(struct command_info commands[])
{
    struct command_info *c;
    memset(commands, 0, sizeof(struct command_info)*CMD_COUNT);

    c = &commands[CMD_EXIT];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_exit);

    c = &commands[CMD_MOVE];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_move);
    c->min_argc = 1;
    c->help_message = help_message_move;
    c->is_need_redraw_after_execution = 1;

    c = &commands[CMD_EXPLORE];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_explore);
    c->is_need_redraw_after_execution = 1;

    c = &commands[CMD_WAIT];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_wait);
    c->is_need_redraw_after_execution = 1;
    
    c = &commands[CMD_INFO_UNIT];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_info_unit);
    c->min_argc = 1;
    c->help_message = help_message_info_unit;

    c = &commands[CMD_INFO_ITEM];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_info_item);
    c->min_argc = 1;
    c->help_message = help_message_info_item;

    c = &commands[CMD_EQUIP];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_equip);
    c->min_argc = 2;
    c->help_message = help_message_equip;

    c = &commands[CMD_UNEQUIP];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_unequip);
    c->min_argc = 2;
    c->help_message = help_message_equip;

    c = &commands[CMD_IMPROVE];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_improve);
    c->min_argc = 2;
    c->help_message = help_message_improve;

    c = &commands[CMD_SHOP_BUY_ITEM];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_buy);
    c->min_argc = 1;
    c->help_message = help_message_buy;

    c = &commands[CMD_SHOP_SELL_ITEM];
    fill_synonyms(c->cmd_synonyms, cmd_synonyms_sell);
    c->min_argc = 1;
    c->help_message = help_message_sell;
}
