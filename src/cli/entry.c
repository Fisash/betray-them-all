#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/entry.h"
#include "cli/base.h"
#include "cli/battle.h"
#include "cli/shop.h"
#include "cli/command_interpretation.h"
#include "cli/command_info.h"
#include "cli/command_parser.h"

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

struct command_inputer inputer;

static void game_input_command(struct game_state *game, const struct game_info *info)
{
    command_inputer_input(&inputer);
    interpret_command(&inputer.cmd, game, info);
}

void cli_run(struct game_state *game, const struct game_info *info)
{
    command_inputer_init(&inputer); /* move to init from main */
    game->is_running = 1;
    while(game->is_running)
    {
        switch (game_state_get_status(game))
        {
            case FIGHTING:
                cli_battle_run(&game->battle, info);
                break;
            case SHOPPING:
                cli_shop_run(&inputer, &game->active_shop, &game->squad);
                break;
            case EVENT_HAPPENING:
                cli_active_event(game, info);
                break;
            case LOSE:
                puts("You lost!");
                game->is_running = 0;
                break;
            case LEAVE:
                puts("Exitting.");
                break;
            default:
                game_input_command(game, info);
                break;
        }
    }
}
