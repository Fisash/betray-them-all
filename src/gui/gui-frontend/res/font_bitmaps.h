#ifndef FONT_BITMAPS_H
#define FONT_BITMAPS_H

#include <stdint.h>

typedef enum {
    FONT_CONSOLE,
    FONT_COUNT
} font_id_t;

typedef struct {
    uint8_t char_pixel_width;
    uint8_t char_pixel_height;
    uint8_t bytes_per_row;
    int is_extended_ascii;
    const unsigned char *data;
} font_t;

void fonts_load(font_t fonts[]);

int font_is_fill_char_pixel(const font_t *font, char c, 
                                 uint8_t x, uint8_t y);

#endif
