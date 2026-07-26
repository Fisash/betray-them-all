#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "cli/game.h"
#include "cli/input.h"

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

struct game_info game_info;
struct game_state game_state;

int main(void)
{
    if(!isatty(1))
    {
        fputs("stdout is not tty attached.\n", stderr);
        exit(1);
    }
    game_info_load(&game_info);
    game_state_init(&game_state, &game_info);

    turn_off_canonical();
    puts("Welcome to \033[1mBetray Them All\033[0m");

    game_run(&game_state, &game_info);

    puts("Goodbye.");
    turn_on_canonical();

    return 0;
}
