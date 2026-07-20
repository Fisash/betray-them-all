#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#include "gui/gui-frontend/res/palletes.h"
#include "gui/gui-frontend/res/sprites.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

typedef struct {
    pallete_t palletes[PALLETE_COUNT];
    sprite_t sprites[SPRITE_COUNT];
    font_t fonts[FONT_COUNT];
} game_resources_t;

void game_resources_load(game_resources_t *res);

#endif
