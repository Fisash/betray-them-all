#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gui/platform_interface.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

typedef struct {
    frame_buffer_t *fb;
    font_t *font;
    int size;
    int has_background;
    uint32_t color_background;
    uint32_t color_foreground;
} text_renderer_t;

void text_renderer_init(text_renderer_t *r, frame_buffer_t *fb, 
                    font_t *font, int size, int has_background, 
         uint32_t color_background, uint32_t color_foreground);

void text_renderer_render(text_renderer_t *r, const char *rendering, 
                                        int x_offset, int y_offset);
#endif
