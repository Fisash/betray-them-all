#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>

#include "gui/gui-frontend/res/palletes.h"

#define SPRITE_CELL_W 16
#define SPRITE_CELL_H 16

enum sprite_id {
    SPRITE_CELL_MEADOW,
    SPRITE_CELL_FOREST,
    SPRITE_CELL_MOUNTAIN,
    SPRITE_CELL_VILLAGE,

    SPRITE_COUNT
};

struct sprite {
    uint16_t width;
    uint16_t height;
    enum pallete_indexing indexing;
    struct pallete *pallete;
    const unsigned char *pixels;
};

void sprites_load(struct sprite sprites[], struct pallete palletes[]);

uint32_t sprite_get_color(const struct sprite *sprite, int x, int y);

#endif
