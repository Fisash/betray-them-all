#include <stdio.h>
#include "game_session.h"

#include "cli/input.h"
#include "cli/battle.h"
#include "cli/shop.h"
#include "cli/command_interpretation.h"
#include "cli/command_info.h"
#include "cli/command_parser.h"

/*-----------------------------------------------------------------------*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* maybe put in header file (core?) */
static const struct event *get_current_event(const struct game *game)
{
    return &game->info->events_info.events[game->state->active_event_id];
}

static void game_handle_current_event(struct game *game)
{
    const struct event *active;
    int options_count, answer, i;

    active = get_current_event(game);
    options_count = event_get_answer_count(active);

    puts(active->title);
    puts(active->message);
    for(i = 0; i < options_count; i++)
       printf("%d. %s\n", i+1, active->answers[i].text);

    answer = input_choose_number(1, options_count);
    event_system_handle_answer(answer-1, game->state, game->info);

    game->state->active_event_id = EVENT_NONE;  /* subprogram */
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-----------------------------------------------------------------------*/

struct game_state GLOBAL_game_state;           /* get rid, rework */

/* add second parameter, when global vars will be vanished */
void game_session_load(struct game *game)
{
    game_state_init(&GLOBAL_game_state, game->info); /* no global! */
    game->state = &GLOBAL_game_state;
}

/*-----------------------------------------------------------------------*/

void game_session_run(struct game *game)
{
    game->is_running = 1;
    game->state->is_running = 1;  /* redundant, get rid */
                                  /* but first get rid from core */

    while(game->is_running && game->state->is_running)
    {
        switch (game_state_get_status(game->state))
        {
            case FIGHTING:
                cli_battle_run(&game->state->battle, game->info);
                break;
            case SHOPPING:
                cli_shop_run(game->inputer, &game->state->active_shop,
                                                  &game->state->squad);
                break;
            case EVENT_HAPPENING:
                game_handle_current_event(game);
                break;
            case LOSE:
                puts("You lost!");
                game->is_running = 0;
                break;
            case LEAVE: /* this is not work */
                break;
            default:
                command_inputer_input(game->inputer);
                apply_command(&game->inputer->cmd, game->state, game->info);
                break;
        }
    }
    puts("Session exit.");
    game->state = NULL;
}

