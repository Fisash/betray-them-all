#ifndef FONT_BITMAPS_H
#define FONT_BITMAPS_H

#define FONT_BYTE_PER_CHAR 8
#define FONT_CHAR_COUNT 128

#include <stdint.h>

extern const unsigned char 
       font_console_8x8[FONT_CHAR_COUNT][FONT_BYTE_PER_CHAR];

int font_console_8x8_mask(char c, uint8_t x, uint8_t y);

#endif
