#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#include "gui/gui-frontend/res/palletes.h"
#include "gui/gui-frontend/res/sprites.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

struct game_resources {
    struct pallete palletes[PALLETE_COUNT];
    struct sprite sprites[SPRITE_COUNT];
    struct font fonts[FONT_COUNT];
};

void game_resources_load(struct game_resources *res);

#endif
