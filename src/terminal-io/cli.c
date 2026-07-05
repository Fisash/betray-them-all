#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "terminal-io/cli.h"

#include "terminal-io/terminal_view.h"
#include "terminal-io/cli_battle.h"

#include "core/time_system.h"
#include "core/exploring_system.h"
#include "core/event_system.h"
#include "core/squad_movement.h"

static const char *args_spliter = " \n";

static void draw(char *framebuffer, draw_frame_context_t *context)
{
    terminal_view_redraw(framebuffer, context);
    terminal_view_stdout_framebuffer(framebuffer);
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
        {
            const unit_template_t *t = &templates[unit->template_id];
            printf("Name: %s (%s)\nhp: %d/%d\nSTR: %d\nAGL: %d\nWIL: "
                                "%d\nINT: %d\nArmor: %s\nWeapon: %s\n",
                         unit->name, t->name, unit->hp, unit->max_hp, 
                         unit->stats.strength, unit->stats.agility, 
                         unit->stats.will, unit->stats.intelligence, 
                         items_info[unit->armor.id].title,
                         items_info[unit->weapon.id].title);
        }
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
    for(int i = 0; i < answer_count; i++)
       printf("%d. %s", i+1, event->answers[i].text);
    putc('\n', stdout);
}


static void cli_active_event(int16_t *active_event_id, battle_state_t *battle, 
                                      squad_t *squad, const game_info_t *info)
{
    const event_t *active = &info->events_info.events[*active_event_id];

    int answer_count = event_get_answer_count(active);
    output_event_info(active, answer_count);
    int answer_index = cli_choose_digit(1, answer_count)-1;

    event_answer_handle_context_t context = {active, answer_index, 
                                             battle, squad, info};
    event_system_handle_answer(context);

    *active_event_id = EVENT_NONE;
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
                          &game_state->active_event_id);
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

/* fill command struct by they ptr and input buffer ptr */
static void parse_command(char *input_buf, command *cmd)
{
    cmd->argc = 0;
    char* current_token = strtok(input_buf, args_spliter);
    while (current_token != NULL && cmd->argc < MAX_AGRC)
    {
        cmd->argv[cmd->argc++] = current_token;
        current_token = strtok(NULL, args_spliter);
    }
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
    while(fgets(input_buf, INPUT_BUF_SIZE, stdin) != NULL)
    {
        parse_command((char*)input_buf, &cmd);
        interpret_command(&cmd, game_state, game_info, &need_redraw);        
        if(game_state->battle.status == BATTLE_STATUS_ACTIVE)
            cli_battle_run(&game_state->battle, game_info);

        if(game_state->active_event_id  != EVENT_NONE)
            cli_active_event(&game_state->active_event_id, &game_state->battle, 
                                            &game_state->squad, game_info);
        if(need_redraw)
        {
            draw_context.days = game_state->days;
            draw((char*)framebuffer, &draw_context);
        }
    }
}
