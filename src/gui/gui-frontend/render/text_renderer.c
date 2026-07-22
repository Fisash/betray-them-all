#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui/gui-frontend/render/text_renderer.h"
#include "gui/gui-frontend/res/font_bitmaps.h"


void text_renderer_init(struct text_renderer *r, char *text,  
                        struct font *font, int size, int has_background, 
                        uint32_t color_background, uint32_t color_foreground)
{
    r->text = text;
    r->font = font;
    r->size = size;
    r->has_background = has_background;
    r->color_background = color_background; 
    r->color_foreground = color_foreground; 
}

static uint16_t strline_count(const char *s)
{
    uint16_t count = 1;
    while(s && *s != 0)
    {
        if(*s == '\n')
            count++;
        s++;
    }
    return count;
}

void text_renderer_calc_layout(struct text_renderer *r)
{
    uint16_t width = (uint16_t)(strlen(r->text) * 
                     r->font->char_pixel_width * r->size);
    uint16_t height = (strline_count(r->text) * 
                      r->font->char_pixel_height * r->size);
    r->layout.out.width = width;
    r->layout.out.height = height;
        
    layout_node_calc(&r->layout);
}

static void render_char(struct text_renderer *r, struct frame_buffer *fb,
                              char rendering, int x_offset, int y_offset)
{
    int x, y, wx, wy, is_set, target_x, target_y;
    uint32_t *pixel;

    for(y = 0; y < r->font->char_pixel_height * r->size; y++)
    {
        for(x = 0; x < r->font->char_pixel_width * r->size; x++)
        {
            target_x = x + x_offset;
            target_y = y + y_offset;

            if(target_x < 0 || target_x >= fb->size.width ||
               target_y < 0 || target_y >= fb->size.height)
                    continue; 

            wx = x / r->size; 
            wy = y / r->size;

            pixel = (fb->data + target_y*fb->size.width + target_x);
            is_set = font_is_fill_char_pixel(r->font, rendering, wx, wy);

            if(is_set)
                *pixel = r->color_foreground;
            else if(r->has_background)
                *pixel = r->color_background;
        }
    }
}

void text_renderer_render(struct text_renderer *r, struct frame_buffer *fb)
{
    char *rendering = r->text;

    int x = r->layout.out.x;
    int y = r->layout.out.y;

    int x_start = x;
    while(*rendering != 0)
    {
        if(*rendering == '\n')
        {
            y += r->font->char_pixel_height*r->size;
            x = x_start;
            rendering++;
            continue;
        }

        render_char(r, fb, *rendering, x, y);
        x += r->font->char_pixel_width*r->size;
        rendering++;
    }
}
