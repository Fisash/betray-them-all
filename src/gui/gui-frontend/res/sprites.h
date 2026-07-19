#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>

#define CELL_SPRITE_W 8
#define CELL_SPRITE_H 16

extern const uint32_t sprite_pallete[16];

extern const unsigned char 
       sprite_cell_meadow[CELL_SPRITE_H][CELL_SPRITE_W];
extern const unsigned char 
       sprite_cell_forest[CELL_SPRITE_H][CELL_SPRITE_W];
extern const unsigned char 
       sprite_cell_mountain[CELL_SPRITE_H][CELL_SPRITE_W];
extern const unsigned char 
       sprite_cell_village[CELL_SPRITE_H][CELL_SPRITE_W];

uint32_t sprite_color(const unsigned char *sprite, int x, int y);

#endif
