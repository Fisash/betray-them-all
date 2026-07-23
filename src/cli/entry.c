#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/entry.h"
#include "cli/base.h"
#include "cli/terminal_view.h"
#include "cli/battle.h"
#include "cli/shop.h"
#include "cli/command_interpretation.h"

static void draw(char *framebuffer, struct draw_frame_context *context)
{
    terminal_view_redraw(framebuffer, context);
    terminal_view_stdout_framebuffer(framebuffer);
}

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

void cli_run(struct game_state *game, const struct game_info *info)
{
    struct draw_frame_context draw_context;
    struct command cmd;
    char *frame, *input;
    int need_redraw;
    frame = buf_frame;
    input = buf_input; 
    terminal_view_init_framebuffer(frame);
    draw_context_init(&draw_context, &game->world, &game->squad,
                      info->cells_info, info->items, game->days);
    need_redraw = 1;
    game->is_running = 1;
    while(game->is_running)
    {
        if(need_redraw)
        {
            draw_context.days = game->days;
            draw(frame, &draw_context);
            need_redraw = 0;
        }
        cli_base_input_command(&cmd, input);
        interpret_command(&cmd, game, info, &need_redraw);        
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
