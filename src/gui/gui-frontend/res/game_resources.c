#include "gui/gui-frontend/res/game_resources.h"

void game_resources_load(struct game_resources *res)
{
    palletes_load(res->palletes);
    sprites_load(res->sprites, res->palletes);
    fonts_load(res->fonts);
}
