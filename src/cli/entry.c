#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "entry.h"
#include "input.h"

void game_entry(int argc, char **argv)
{
    struct termios term;
    if(!isatty(1))
    {
        perror("betray");
        exit(1);
    }
    tcgetattr(1, &term);

    term.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(1, 0, &term);

    simple_shell(" / ");

    term.c_lflag |= ECHO|ICANON;
    tcsetattr(1, 0, &term);
}
