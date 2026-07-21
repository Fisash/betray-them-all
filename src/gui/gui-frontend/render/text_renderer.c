#include "gui/gui-frontend/render/text_renderer.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

void text_renderer_init(struct text_renderer *r, struct frame_buffer *fb, 
                        struct font *font, int size, int has_background, 
                        uint32_t color_background, uint32_t color_foreground)
{
    r->fb = fb;
    r->font = font;
    r->size = size;
    r->has_background = has_background;
    r->color_background = color_background; 
    r->color_foreground = color_foreground; 
}

static void render_char(struct text_renderer *r, char rendering, 
                                     int x_offset, int y_offset)
{
    int x, y, wx, wy, is_set;
    uint32_t *pixel;

    for(y = 0; y < r->font->char_pixel_height * r->size; y++)
    {
        for(x = 0; x < r->font->char_pixel_width * r->size; x++)
        {
            wx = x / r->size; 
            wy = y / r->size;

            pixel = (r->fb->data + (y+y_offset)*r->fb->size.width + x+x_offset);
            is_set = font_is_fill_char_pixel(r->font, rendering, wx, wy);
            if(is_set)
                *pixel = r->color_foreground;
            else if(r->has_background)
                *pixel = r->color_background;
        }
    }
}

void text_renderer_render(struct text_renderer *r, const char *rendering, 
                                              int x_offset, int y_offset)
{
    int x_start = x_offset;
    while(*rendering != 0)
    {
        if(*rendering == '\n')
        {
            y_offset += r->font->char_pixel_height*r->size;
            x_offset = x_start;
            rendering++;
            continue;
        }

        render_char(r, *rendering, x_offset, y_offset);
        x_offset += r->font->char_pixel_width*r->size;
        rendering++;
    }
}
