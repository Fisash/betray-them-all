#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli/command_interpretation.h"
#include "cli/print.h"

#define HELP_MSG_INFO    "info [unit|item]"
#define HELP_MSG_UNEQUIP "unequip [N unit] [weapon|armor]"
#define HELP_MSG_EQUIP   "equip [N unit] [N item]"
#define HELP_MSG_IMPROVE "improve [N unit] [str|agl|wil|int]"
#define HELP_MSG_MOVE    "move [r|l|u|d|s]"

/*-----------------------------------------------------------------------*/


static void interpret_unit_info(struct command *cmd, struct squad *squad, 
                                  const struct unit_template templates[],
                                     const struct item_info items_info[])
{
    if(cmd->argc < 3) 
    {
        puts("Identify unit to put info");
        return;
    }
    
    uint8_t unit_num = atoi(cmd->argv[2]);
    struct unit *unit = squad_get_unit_by_num(squad, unit_num);
    if(unit && unit->is_alive)
        print_unit_info(unit, items_info, templates);
}

static void interpret_item_info(struct command *cmd, struct squad *squad, 
                                     const struct item_info items_info[])
{
    if(cmd->argc < 3) 
    {
        puts("Identify item to put info.");
        return;
    }
    
    uint8_t item_num = atoi(cmd->argv[2]);
    struct item *item = squad_get_item_by_num(squad, item_num);
    if(item && item->id != ITEM_NONE)
        print_item_info(item, items_info, NULL);
}

static void interpret_info(struct command *cmd, struct squad *squad, 
                             const struct unit_template templates[],
                                const struct item_info items_info[])
{
    if(cmd->argc < 2)
    {
        puts(HELP_MSG_INFO);
        return;
    }

    if(strcmp(cmd->argv[1], "unit") == 0)
        interpret_unit_info(cmd, squad, templates, items_info); 
    else if(strcmp(cmd->argv[1], "item") == 0)
        interpret_item_info(cmd, squad, items_info);
}


static void interpret_unequip(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts(HELP_MSG_UNEQUIP);
        return;
    }
    uint8_t unit_num;
    enum item_type equipment_type = ITEM_TYPE_GENERIC;
    enum squad_unit_unequip_status result;

    unit_num = atoi(cmd->argv[1]);
    if(strcmp(cmd->argv[2], "weapon") == 0)
        equipment_type = ITEM_TYPE_WEAPON;
    else if(strcmp(cmd->argv[2], "armor") == 0)
        equipment_type = ITEM_TYPE_ARMOR;

    result = squad_unit_unequip(squad, unit_num, equipment_type);

    switch(result)
    {
        case SQUAD_UNIT_UNEQUIP_OK:
            puts("Item unequiped!");
            break;
        case SQUAD_UNIT_UNEQUIP_INVALID_UNIT:
            puts("Invalid unit");
            break;
        case SQUAD_UNIT_UNEQUIP_INVALID_TYPE:
            puts("Invalid equipment type");
            break;
        case SQUAD_UNIT_UNEQUIP_ALREADY_REMOVED:
            puts("Unit does no have this type equipment");
            break;
        default:
            break;
    }
}


static void interpret_equip(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts(HELP_MSG_EQUIP);
        return;
    }
    uint8_t unit_num, item_num;
    enum squad_unit_equip_status result;

    unit_num = atoi(cmd->argv[1]);
    item_num = atoi(cmd->argv[2]);

    struct item *item = squad_get_item_by_num(squad, item_num);
    result = squad_unit_equip(squad, unit_num, item);

    switch(result)
    {
        case SQUAD_UNIT_EQUIP_OK:
            puts("Item equiped!");
            break;
        case SQUAD_UNIT_EQUIP_INVALID_UNIT:
            puts("Invalid unit");
            break;
        case SQUAD_UNIT_EQUIP_INVALID_ITEM:
            puts("Invalid item");
            break;
        default:
            break;
    }

}


void interpret_unit_improve(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts(HELP_MSG_IMPROVE);
        return;
    }

    int status;
    enum stat_selection stat;
    uint8_t unit_num;

    unit_num = atoi(cmd->argv[1]);
    struct unit *unit = squad_get_unit_by_num(squad, unit_num);

    if(strcmp(cmd->argv[2], "str") == 0)
        stat = STRENGTH;
    else if(strcmp(cmd->argv[2], "agl") == 0)
        stat = AGILITY;
    else if(strcmp(cmd->argv[2], "wil") == 0)
        stat = WILL;
    else if(strcmp(cmd->argv[2], "int") == 0)
        stat = INTELLIGENCE;
    else
        return;

    status = unit_apply_stat_point(unit, stat);
    puts((status == 0) ? "Stat improved!" : "No points");
}


static void interpret_move(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 2)
    {
        puts(HELP_MSG_MOVE);
        return;
    }
    switch (*cmd->argv[1])
    {
        case 'r':
            squad->move_order = MOVE_RIGHT;
            break;
        case 'l':
            squad->move_order = MOVE_LEFT;
            break;
        case 'u':
            squad->move_order = MOVE_UP;
            break;
        case 'd':
            squad->move_order = MOVE_DOWN;
            break;
        case 's':
            squad->move_order = MOVE_NONE;
            break;
    }
}

/*-----------------------------------------------------------------------*/

/* maybe some better place for that */
/* but i can't put it in string.h :< */
static int match_string(const char *s, const char *strings[])
{
    int i = 0;
    while(strings[i] != NULL)
    {
        if(strcmp(s, strings[i]) == 0)
            return 1;
        else
            i++;
    }
    return 0;
}

/*-----------------------------------------------------------------------*/

const char *cmd_synonyms_exit[]    = { "exit", "quit", "q", NULL };
const char *cmd_synonyms_move[]    = { "move", "mov", "m", NULL };
const char *cmd_synonyms_explore[] = { "explore", "e", NULL };
const char *cmd_synonyms_next[]    = { "next", NULL };
const char *cmd_synonyms_info[]    = { "info", NULL };
const char *cmd_synonyms_equip[]   = { "equip", NULL };
const char *cmd_synonyms_unequip[] = { "unequip", NULL };
const char *cmd_synonyms_improve[] = { "improve", NULL };
const char *cmd_synonyms_help[]    = { "help", "?", NULL };

enum command_type {
    cmd_none,
    cmd_exit,
    cmd_move,
    cmd_explore,
    cmd_next,
    cmd_info,
    cmd_equip,
    cmd_unequip,
    cmd_improve,
    cmd_help,
    cmd_unknown
};

/* maybe learn and use suffix tree for less complexity */
static enum command_type interpret_string_command(const char *s)
{
    if(s == NULL || strlen(s) == 0)
       return cmd_none;
    else if(match_string(s, cmd_synonyms_exit))
       return cmd_exit;
    else if(match_string(s, cmd_synonyms_move))
       return cmd_move;
    else if(match_string(s, cmd_synonyms_explore))
       return cmd_explore;
    else if(match_string(s, cmd_synonyms_next))
       return cmd_next;
    else if(match_string(s, cmd_synonyms_info))
       return cmd_info;
    else if(match_string(s, cmd_synonyms_equip))
       return cmd_equip;
    else if(match_string(s, cmd_synonyms_unequip))
       return cmd_unequip;
    else if(match_string(s, cmd_synonyms_improve))
       return cmd_improve;
    else if(match_string(s, cmd_synonyms_help))
       return cmd_help;
    else
       return cmd_unknown;
}

void interpret_command(struct command *cmd, struct game_state *game,
                       const struct game_info *info, int *need_redraw)
{
    switch (interpret_string_command(cmd->argv[0]))
    {
        case cmd_exit:
            game->is_running = 0;
            break;
        case cmd_move:
            interpret_move(cmd, &game->squad);
            time_system_spend(game, 1);    /* <- take this case apart! */        
            *need_redraw = 1;
            break;
        case cmd_explore:
            exploring_system_explore_squad_cell(&game->squad, 
                                                &game->world, 
                                                &info->events_info, 
                                                &game->active_event_id,
                                                info->cells_info);
            *need_redraw = 1;
            break;
        case cmd_next:
            time_system_spend(game, 1);
            break;
        case cmd_info:
            interpret_info(cmd, &game->squad, 
                           info->unit_templates,
                           info->items);
            break;
        case cmd_equip:
            interpret_equip(cmd, &game->squad);
            break;
        case cmd_unequip:
            interpret_unequip(cmd, &game->squad);
            break;
        case cmd_improve:
            interpret_unit_improve(cmd, &game->squad);
            break;
        case cmd_help:
            /* write help message */
            break;
        case cmd_unknown:
            /* maybe write something */
            break;
        case cmd_none:
        default:
            break;
    }
}
