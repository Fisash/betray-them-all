#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gui/frame_buffer.h"
#include "gui/gui-frontend/res/font_bitmaps.h"
#include "gui/gui-frontend/layout_node.h"

struct text_renderer {
    struct layout_node layout;

    char *text;
    const struct font *font;
    int size;
    int has_background;
    uint32_t color_background;
    uint32_t color_foreground;
};

void text_renderer_init(struct text_renderer *r, char *text,  
                        const struct font *font, int size, 
                        int has_background, uint32_t color_background, 
                        uint32_t color_foreground);

void text_renderer_calc_layout(struct text_renderer *r);

void text_renderer_render(struct text_renderer *r, struct frame_buffer *fb);

#endif
