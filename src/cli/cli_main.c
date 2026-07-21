#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/cli_main.h"
#include "cli/cli_base.h"
#include "cli/terminal_view.h"
#include "cli/cli_battle.h"
#include "cli/cli_shop.h"

#include "core/time_system.h"
#include "core/exploring_system.h"
#include "core/event_system.h"
#include "core/squad_movement.h"


static void draw(char *framebuffer, struct draw_frame_context *context)
{
    terminal_view_redraw(framebuffer, context);
    terminal_view_stdout_framebuffer(framebuffer);
}

static void print_unit_info(struct unit *unit, const struct item_info items[],
                                  const struct unit_template templates[])
{
    printf("Name: %s, (%s)\n", unit->name, 
       templates[unit->template_id].name);
    printf("LVL: %d. EXP: %d/%d\n", unit->level, unit->exp, 
                                 unit->exp_for_next_level);
    printf("HP: %d/%d\n", unit->hp, unit->max_hp);
    printf("STR: %d\nAGL: %d\nWIL: %d\nINT: %d\n", 
        unit->stats.strength, unit->stats.agility, 
        unit->stats.will,unit->stats.intelligence);

    printf("Weapon: ");
    print_item_info(&unit->weapon, items, unit);
    printf("Armor: ");
    print_item_info(&unit->armor, items, unit);
    if(unit->unspent_stat_points > 0)
        printf("Unspent stat leveling points: %d", 
                       unit->unspent_stat_points);
}

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
        puts("Identify item to put info");
        return;
    }
    
    uint8_t item_num = atoi(cmd->argv[2]);
    struct item *item = squad_get_item_by_num(squad, item_num);
    if(item && item->id != ITEM_NONE)
        print_item_info(item, items_info, NULL);
}
/* struct command info*/
static void interpret_info(struct command *cmd, struct squad *squad, 
                           const struct unit_template templates[],
                              const struct item_info items_info[])
{
    if(cmd->argc < 2) return;

    if(strcmp(cmd->argv[1], "unit") == 0)
        interpret_unit_info(cmd, squad, templates, items_info); 

    if(strcmp(cmd->argv[1], "item") == 0)
        interpret_item_info(cmd, squad, items_info);
}


/* command unequip*/
static void interpret_unequip(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts("Identify unit and equipment type like this:"
             "unequip [squad unit number] [weapon/armor]");
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

/* command equip*/
static void interpret_equip(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts("Identify unit and item like this:"
             "equip [squad unit number] [squad item number]");
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

/* command improve*/
void interpret_unit_improve(struct command *cmd, struct squad *squad)
{
    if(cmd->argc < 3)
    {
        puts("Identify unit and stat like this:"
             "improve [squad unit number] [str/agl/wil/int]");
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

/* command mov*/
static void interpret_move(struct command *cmd, struct squad *squad)
{
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

static void output_event_info(const struct event *event, uint8_t answer_count)
{
    puts(event->title);
    puts(event->message);
    int i;
    for(i = 0; i < answer_count; i++)
       printf("%d. %s\n", i+1, event->answers[i].text);
    putc('>', stdout);
}


static void cli_active_event(struct game_state *state, const struct game_info *info)
{
    const struct event *active = 
                  &info->events_info.events[state->active_event_id];
    int answer_count = event_get_answer_count(active);
    output_event_info(active, answer_count);
    int answer_index = cli_base_choose_number(1, answer_count)-1;

    event_system_handle_answer(answer_index, state, info);

    state->active_event_id = EVENT_NONE;
}

static void interpret_command(struct command *cmd, struct game_state *game_state,
                      const struct game_info *game_info, int *need_redraw)
{
    if((strcmp(cmd->argv[0], "exit") == 0) ||
       (strcmp(cmd->argv[0], "quit") == 0) ||
       (strcmp(cmd->argv[0], "q") == 0))
            exit(0);

    *need_redraw = 1;

    if((strcmp(cmd->argv[0], "mov") == 0 || 
       (strcmp(cmd->argv[0], "m") == 0)) &&
                          cmd->argc > 1)
    {
        interpret_move(cmd, &game_state->squad);
        time_system_spend(game_state, 1);
    }

    if(strcmp(cmd->argv[0], "explore") == 0)
       exploring_system_explore_squad_cell(&game_state->squad, 
                          &game_state->world, 
                          &game_info->events_info, 
                          &game_state->active_event_id,
                          game_info->cells_info);

    if(strcmp(cmd->argv[0], "next") == 0)
        time_system_spend(game_state, 1);

    if(strcmp(cmd->argv[0], "info") == 0)
    {
        interpret_info(cmd, &game_state->squad, 
                      (const struct unit_template*)&game_info->unit_templates,
                      (const struct item_info*)&game_info->items);
        *need_redraw = 0;
    }

    if(strcmp(cmd->argv[0], "equip") == 0)
    {
        interpret_equip(cmd, &game_state->squad);
        *need_redraw = 0;
    }

    if(strcmp(cmd->argv[0], "unequip") == 0)
    {
        interpret_unequip(cmd, &game_state->squad);
        *need_redraw = 0;
    }

    if(strcmp(cmd->argv[0], "improve") == 0)
    {
        interpret_unit_improve(cmd, &game_state->squad);
        *need_redraw = 0;
    }

}

static int has_pending_actions(const struct game_state *state)
{
    return (state->battle.status == BATTLE_STATUS_ACTIVE ||
            state->active_event_id != EVENT_NONE         ||
            state->active_shop != NULL                   );
}

void check_game_status(const struct game_state *state)
{
    if(state->is_over)
    {
        puts("You lost!");
        exit(0);
    }
}

void cli_run(struct game_state *game_state, const struct game_info *game_info)
{
    char framebuffer[FRAME_HEIGHT][FRAME_WIDTH];
    terminal_view_init_framebuffer((char*)framebuffer);

    struct draw_frame_context draw_context = {&game_state->world, 
                   &game_state->squad, (struct cell_info*)&game_info->cells_info, 
                        (struct item_info *)&game_info->items, game_state->days};

    draw((char*)framebuffer, &draw_context);

    char input_buf[INPUT_BUF_SIZE];
    struct command cmd;
    int need_redraw = 0;
    for(;;)
    {
        cmd = cli_base_input_command(input_buf);

        interpret_command(&cmd, game_state, game_info, &need_redraw);        
        check_game_status(game_state);

        while (has_pending_actions(game_state))
        {
            if(game_state->battle.status == BATTLE_STATUS_ACTIVE)
                cli_battle_run(&game_state->battle, game_info);
    
            if(game_state->active_shop != NULL)
                cli_shop_run(&game_state->active_shop, &game_state->squad, 
                                                        game_info->items);

            if(game_state->active_event_id  != EVENT_NONE)
                cli_active_event(game_state, game_info);
        }
             

        if(need_redraw)
        {
            draw_context.days = game_state->days;
            draw((char*)framebuffer, &draw_context);
        }
    }
}
