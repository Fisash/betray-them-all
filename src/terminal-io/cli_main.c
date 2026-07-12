#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "terminal-io/cli_main.h"
#include "terminal-io/cli_base.h"
#include "terminal-io/terminal_view.h"
#include "terminal-io/cli_battle.h"
#include "terminal-io/cli_shop.h"

#include "core/time_system.h"
#include "core/exploring_system.h"
#include "core/event_system.h"
#include "core/squad_movement.h"


static void draw(char *framebuffer, draw_frame_context_t *context)
{
    terminal_view_redraw(framebuffer, context);
    terminal_view_stdout_framebuffer(framebuffer);
}

static void fill_scalings_view(char out[], scaling_group_t s)
{
    out[0] = scale_get_rank_view(s.strength);
    out[1] = scale_get_rank_view(s.agility);
    out[2] = scale_get_rank_view(s.will);
    out[3] = scale_get_rank_view(s.intelligence);
    out[4] = 0;
}

static void print_unit_info(unit_t *unit, const item_info_t items[],
                                  const unit_template_t templates[])
{
    printf("Name: %s, (%s)\n", unit->name, 
       templates[unit->template_id].name);
    printf("HP: %d/%d\n", unit->hp, unit->max_hp);
    printf("STR: %d\nAGL: %d\nWIL: %d\nINT: %d\n", 
        unit->stats.strength, unit->stats.agility, 
        unit->stats.will,unit->stats.intelligence);

    char damage_scalings[5], crit_scalings[5];
    const item_info_t *weapon = &items[unit->weapon.id];
    fill_scalings_view(damage_scalings, 
                       weapon->props.weapon.damage.scalings);
    fill_scalings_view(crit_scalings, 
                       weapon->props.weapon.crit.scalings);
    printf("Weapon: %s - %d(%d) damage (%s),"
                        "%d(%d) crit (%s)\n",
        weapon->title, unit->weapon.props.weapon.damage, 
        unit_get_damage(unit, items), damage_scalings,
        unit->weapon.props.weapon.crit, 
        unit_get_crit(unit, items), crit_scalings);

    char protection_scalings[5], mobility_scalings[5];
    const item_info_t *armor = &items[unit->armor.id];
    fill_scalings_view(protection_scalings, 
                       armor->props.armor.protection.scalings);
    fill_scalings_view(mobility_scalings, 
                       armor->props.armor.mobility.scalings);
    printf("Armor: %s - %d(%d) protection (%s),"
                        "%d(%d) mobility (%s)\n",
        armor->title, unit->armor.props.armor.protection, 
        unit_get_protection(unit, items), protection_scalings,
        unit->armor.props.armor.mobility, 
        unit_get_mobility(unit, items), mobility_scalings);
}

static void interpret_unit_info(command *cmd, squad_t *squad, 
                           const unit_template_t templates[],
                              const item_info_t items_info[])
{
    if(cmd->argc < 3) 
    {
      puts("Identify unit to put info");
      return;
    }
    
    int unit_id = atoi(cmd->argv[2]);
    if(unit_id > 0 && unit_id <= MAX_UNITS)
    {
        unit_t *unit = &squad->units[unit_id-1];
        if(unit->is_alive)
            print_unit_info(unit, items_info, templates);
    }
}

/* command info*/
static void interpret_info(command *cmd, squad_t *squad, 
                           const unit_template_t templates[],
                              const item_info_t items_info[])
{
    if(cmd->argc < 2) return;

    if(strcmp(cmd->argv[1], "unit") == 0)
        interpret_unit_info(cmd, squad, templates, items_info); 
}


/* command mov*/
static void interpret_move(command *cmd, squad_t *squad)
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

static void output_event_info(const event_t *event, uint8_t answer_count)
{
    puts(event->title);
    puts(event->message);
    int i;
    for(i = 0; i < answer_count; i++)
       printf("%d. %s\n", i+1, event->answers[i].text);
    putc('>', stdout);
}


static void cli_active_event(game_state_t *state, const game_info_t *info)
{
    const event_t *active = 
                  &info->events_info.events[state->active_event_id];
    int answer_count = event_get_answer_count(active);
    output_event_info(active, answer_count);
    int answer_index = cli_base_choose_number(1, answer_count)-1;

    event_system_handle_answer(answer_index, state, info);

    state->active_event_id = EVENT_NONE;
}

static void interpret_command(command *cmd, game_state_t *game_state,
                      const game_info_t *game_info, int *need_redraw)
{
    if(strcmp(cmd->argv[0], "exit") == 0)
        exit(0);

    if(strcmp(cmd->argv[0], "mov") == 0 && cmd->argc > 1)
    {
        interpret_move(cmd, &game_state->squad);
        time_system_spend(&game_state->squad, 
                          &game_state->world,
                        1, &game_state->days);
        *need_redraw = 1;
    }

    if(strcmp(cmd->argv[0], "explore") == 0)
    {
       exploring_system_explore_squad_cell(&game_state->squad, 
                          &game_state->world, 
                          &game_info->events_info, 
                          &game_state->active_event_id,
                          game_info->cells_info);
        *need_redraw = 1;
    }

    if(strcmp(cmd->argv[0], "next") == 0)
    {
        time_system_spend(&game_state->squad, 
                          &game_state->world,
                        1, &game_state->days);
        *need_redraw = 1;
    }

    if(strcmp(cmd->argv[0], "info") == 0)
    {
        interpret_info(cmd, &game_state->squad, 
                      (const unit_template_t*)&game_info->unit_templates,
                      (const item_info_t*)&game_info->items);
        *need_redraw = 0;
    }
}

static int has_pending_actions(const game_state_t *state)
{
    return (state->battle.status == BATTLE_STATUS_ACTIVE ||
            state->active_event_id != EVENT_NONE         ||
            state->active_shop != NULL                   );
}

void cli_run(game_state_t *game_state, const game_info_t *game_info)
{
    char framebuffer[FRAME_HEIGHT][FRAME_WIDTH];
    terminal_view_init_framebuffer((char*)framebuffer);

    draw_frame_context_t draw_context = {&game_state->world, 
                   &game_state->squad, (cell_info_t*)&game_info->cells_info, 
                        (item_info_t *)&game_info->items, game_state->days};

    draw((char*)framebuffer, &draw_context);

    char input_buf[INPUT_BUF_SIZE];
    command cmd;
    int need_redraw = 0;
    for(;;)
    {
        cmd = cli_base_input_command(input_buf);

        interpret_command(&cmd, game_state, game_info, &need_redraw);        

        while (has_pending_actions(game_state))
        {
            if(game_state->battle.status == BATTLE_STATUS_ACTIVE)
                cli_battle_run(&game_state->battle, game_info);
    
            if(game_state->active_event_id  != EVENT_NONE)
                cli_active_event(game_state, game_info);
    
            if(game_state->active_shop != NULL)
                cli_shop_run(&game_state->active_shop, &game_state->squad, 
                                                        game_info->items);
        }
             

        if(need_redraw)
        {
            draw_context.days = game_state->days;
            draw((char*)framebuffer, &draw_context);
        }
    }
}
