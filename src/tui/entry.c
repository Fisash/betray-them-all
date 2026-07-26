#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/entry.h"
#include "cli/base.h"
#include "cli/terminal_view.h"
#include "cli/battle.h"
#include "cli/shop.h"
#include "cli/command_interpretation.h"
#include "cli/command_info.h"
#include "cli/syntax_parser.h"

static void output_event_info(const struct event *event, uint8_t answer_count)
{
    puts(event->title);
    puts(event->message);
    int i;
    for(i = 0; i < answer_count; i++)
       printf("%d. %s\n", i+1, event->answers[i].text);
    putchar('>');
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


enum {
    leave,
    lose,
    fighting,
    shopping,
    event_happening,
    idle
};

static int current_game_state(const struct game_state *state)
{
    if(!state->is_running)
        return leave;
    else if(state->is_over)
        return lose;
    else if(state->battle.status == BATTLE_STATUS_ACTIVE)
        return fighting;
    else if(state->active_shop)
        return shopping;
    else if(state->active_event_id != EVENT_NONE)
        return event_happening;
    else
        return idle;
}

static char buf_frame[FRAME_HEIGHT*FRAME_WIDTH];
static char buf_input[INPUT_BUF_SIZE];

struct command_info commands_info[CMD_COUNT];

void cli_run(struct game_state *game, const struct game_info *info)
{
    struct draw_frame_context draw_context;
    struct parsed_command cmd;
    char *frame = buf_frame;
    char *input = buf_input; 
    commands_info_init(commands_info);
    terminal_view_init_framebuffer(frame);
    draw_context_init(&draw_context, &game->world, &game->squad,
                    info->cells_info, info->items, &game->days);
    game->is_running = 1;
    while(game->is_running)
    {
        syntax_command_input(&cmd, input, commands_info);
        interpret_command(&cmd, game, info, commands_info);
        if(commands_info[cmd.type].is_need_redraw_after_execution)
        {
            terminal_view_redraw(frame, &draw_context);
            terminal_view_stdout_framebuffer(frame);
        }

        switch (current_game_state(game))
        {
            case fighting:
                cli_battle_run(&game->battle, info);
                break;
            case shopping:
                cli_shop_run(&game->active_shop, &game->squad, info->items);
                break;
            case event_happening:
                cli_active_event(game, info);
                break;
            case lose:
                puts("You lost!");
                game->is_running = 0;
                break;
            case leave:
                puts("Exitting.");
                break;
            case idle:
            default:
                break;
        }
    }
}
