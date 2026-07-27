#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "cli/game.h"
#include "cli/game_menu.h"

/*-----------------------------------------------------------------------*/

static void turn_off_canonical(void)
{
    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(1, 0, &term);
}

static void turn_on_canonical(void)
{
    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag |= ECHO|ICANON;
    tcsetattr(1, 0, &term);
}

/*-----------------------------------------------------------------------*/

struct command_inputer GLOBAL_inputer;         /* get rid, rework */
struct game_info GLOBAL_game_info;             /* get rid, rework */

void game_init(struct game *game)
{
    game_info_load(&GLOBAL_game_info);         /* get rid, rework */
    command_inputer_init(&GLOBAL_inputer);     /* get rid, rework */

    game->info = &GLOBAL_game_info;
    game->inputer = &GLOBAL_inputer;
    game->state = NULL;
    game->is_running = 0;
}

/*-----------------------------------------------------------------------*/

void game_run(struct game *game)
{
    if(!isatty(0) || !isatty(1))
    {
        fputs("io device is not tty attached!\n", stderr);
        exit(1);
    }
    turn_off_canonical();
    puts("Welcome to \033[1mBetray Them All\033[0m!");
    game->is_running = 1;
    game_menu(game);
    turn_on_canonical();
}
