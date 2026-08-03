#include "gui/gui-frontend/game.h"

int main(void)
{
    struct game game;
    game_init(&game);
    game_run(&game);
    return 0;
}
