#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gui/platform_interface.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

struct text_renderer {
    struct frame_buffer *fb;
    struct font *font;
    int size;
    int has_background;
    uint32_t color_background;
    uint32_t color_foreground;
};

void text_renderer_init(struct text_renderer *r, struct frame_buffer *fb, 
                        struct font *font, int size, int has_background, 
                        uint32_t color_background, uint32_t color_foreground);

void text_renderer_render(struct text_renderer *r, const char *rendering, 
                                              int x_offset, int y_offset);
#endif
