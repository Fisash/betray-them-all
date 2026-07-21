#ifndef FONT_BITMAPS_H
#define FONT_BITMAPS_H

#include <stdint.h>

enum font_id {
    FONT_CONSOLE,
    FONT_COUNT
};

struct font {
    uint8_t char_pixel_width;
    uint8_t char_pixel_height;
    uint8_t bytes_per_row;
    int is_extended_ascii;
    const unsigned char *data;
};

void fonts_load(struct font fonts[]);

int font_is_fill_char_pixel(const struct font *font, char c, 
                                       uint8_t x, uint8_t y);

#endif
