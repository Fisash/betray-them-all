#include "gui/gui-frontend/res/game_resources.h"

void game_resources_load(game_resources_t *res)
{
    palletes_load(res->palletes);
    sprites_load(res->sprites, res->palletes);
    fonts_load(res->fonts);
}
