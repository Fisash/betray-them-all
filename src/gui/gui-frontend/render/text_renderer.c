#include "gui/gui-frontend/render/text_renderer.h"
#include "gui/gui-frontend/res/font_bitmaps.h"

static void render_char(frame_buffer_t *fb, char rendering, 
                     int x_offset, int y_offset, int scale, 
                                         int is_background)
{
    int x, y, wx, wy, is_set;
    uint32_t *pixel;

    for(y = 0; y < FONT_CHAR_HEIGHT*scale; y++)
    {
        for(x = 0; x < FONT_CHAR_WIDTH*scale; x++)
        {
            wx = x / scale; 
            wy = y / scale;

            pixel = (fb->data + (y+y_offset)*fb->size.width + x+x_offset);
            is_set = font_console_8x8_mask(rendering, wx, wy);
            if(is_set)
                *pixel = 0x000000U;
            else if(is_background)
                *pixel = 0xFFFFFFU;
        }
    }
}

void text_renderer_render(frame_buffer_t *fb, const char *rendering, 
                              int x_offset, int y_offset, int scale, 
                                                  int is_background)
{
    while(*rendering != 0)
    {
        render_char(fb, *rendering, x_offset, y_offset, 
                                 scale, is_background);
        x_offset += FONT_CHAR_WIDTH*scale;
        rendering++;
    }
}
