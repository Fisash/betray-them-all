#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>

#include "gui/gui-frontend/res/palletes.h"

#define SPRITE_CELL_W 16
#define SPRITE_CELL_H 16

typedef enum {
    SPRITE_CELL_MEADOW,
    SPRITE_CELL_FOREST,
    SPRITE_CELL_MOUNTAIN,
    SPRITE_CELL_VILLAGE,

    SPRITE_COUNT
} sprite_id_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    pallete_indexing_t indexing;
    pallete_t *pallete;
    const unsigned char *pixels;
} sprite_t;

void sprites_load(sprite_t sprites[], pallete_t palletes[]);

uint32_t sprite_get_color(const sprite_t *sprite, int x, int y);

#endif
