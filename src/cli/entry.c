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
                                    /* allocate .bss with this size? */
                                    /* instead of stack */

    terminal_view_init_framebuffer((char*)framebuffer);

    struct draw_frame_context draw_context = {
        &game_state->world, &game_state->squad,
        (struct cell_info*)&game_info->cells_info,
        (struct item_info *)&game_info->items,
        game_state->days
    };

    draw((char*)framebuffer, &draw_context);

    char input_buf[INPUT_BUF_SIZE]; /* allocate .bss with this size? */
                                    /* instead of stack */

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
            need_redraw = 0;
        }
    }
}
