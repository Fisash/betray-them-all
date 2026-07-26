#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli/command_interpretation.h"
#include "cli/print.h"

#define MSG_UNKNOWN_COMMAND "Unknown command!"
#define MSG_HELP_GENERAL \
    "[move] [explore] [map] [info] [item(s)] [unit(s)] [(un)equip]"

static void interpret_move(struct parsed_command *cmd, struct squad *squad)
{
    squad->move_order = cmd->args.move_order;
}

static void interpret_unit_info(struct parsed_command *cmd, 
                                struct squad *squad, 
                                const struct unit_template templates[], 
                                const struct item_info items_info[])
{
    struct unit *unit = squad_get_unit_by_num(squad, cmd->args.unit_num);
    if(unit && unit->is_alive)
        print_unit_info(unit, items_info, templates);
}

static void interpret_item_info(struct parsed_command *cmd, 
                                struct squad *squad, 
                                const struct item_info items_info[])
{
    struct item *item = squad_get_item_by_num(squad, cmd->args.item_num);
    if(item && item->id != ITEM_NONE)
        print_item_info(item, items_info, NULL);
}

static void interpret_equip(struct parsed_command *cmd, 
                                   struct squad *squad)
{
    enum squad_unit_equip_status result;
    uint16_t unit_num = cmd->args.equip.unit_num;
    uint16_t item_num = cmd->args.equip.item_num;

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

static void interpret_unequip(struct parsed_command *cmd, 
                                     struct squad *squad)
{
    enum squad_unit_unequip_status result; 
    uint16_t unit_num = cmd->args.unequip.unit_num;
    enum item_type equipment_type = cmd->args.unequip.equipment_type;

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

void interpret_unit_improve(struct parsed_command *cmd, 
                                   struct squad *squad)
{
    int result;
    uint16_t unit_num = cmd->args.improve.unit_num;
    enum stat_selection stat = cmd->args.improve.stat;

    struct unit *unit = squad_get_unit_by_num(squad, unit_num);
    result = unit_apply_stat_point(unit, stat);
    puts((result == 0) ? "Stat improved!" : "No points");
}

#if 0
void print_help_general(void)
{
   int i;
   for (i=0;i<  ;i++)
}
#endif


/*-----------------------------------------------------------------------*/

void interpret_command(struct parsed_command *cmd, struct game_state *game,
                                              const struct game_info *info)
{
    if (!cmd->is_correct)
    {
        puts(cmd->info->help_message);
        return;
    }

    switch (cmd->type)
    {
        case CMD_EXIT:
            game->is_running = 0;
            break;
        case CMD_MOVE:
            interpret_move(cmd, &game->squad);
            time_system_spend(game, 1);
            break;
        case CMD_EXPLORE:
            exploring_system_explore_squad_cell(&game->squad, 
                            &game->world, &info->events_info, 
                   &game->active_event_id, info->cells_info);
            break;
        case CMD_WAIT:
            time_system_spend(game, 1);
            break;
        case CMD_INFO_WORLD:
            print_world(&game->world, &game->squad);
            break;
        case CMD_SQUAD_INFO_BASE:
            print_base_info(game, info);
            break;
        case CMD_SQUAD_INFO_UNIT:
            interpret_unit_info(cmd, &game->squad, 
                info->unit_templates, info->items);
            break;
        case CMD_SQUAD_INFO_ITEM:
            interpret_item_info(cmd, &game->squad, info->items);
            break;
        case CMD_SQUAD_INFO_INV:
            print_item_list(&game->squad);
            break;
        case CMD_SQUAD_INFO_UNITS:
            print_unit_list(&game->squad);
            break;
        case CMD_EQUIP:
            interpret_equip(cmd, &game->squad);
            break;
        case CMD_UNEQUIP:
            interpret_unequip(cmd, &game->squad);
            break;
        case CMD_IMPROVE:
            interpret_unit_improve(cmd, &game->squad);
            break;
        case CMD_HELP:
            puts(MSG_HELP_GENERAL);
#if 0
            print_help_general(void);
#endif
            break;
        case CMD_UNKNOWN:
            puts(MSG_UNKNOWN_COMMAND);
            break;
        default:
            break;
    }
}
