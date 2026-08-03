#include "gui/gui-frontend/render/panel_renderer.h"

void panel_renderer_render(struct panel_renderer *p, 
                            struct frame_buffer *fb)
{
    uint32_t *pixel;
    int x, y;
    struct layout_node_output *l = &p->layout.out;

    for(y = l->y; y < l->y + l->height; y++)
    {
        for(x = l->x; x < l->x + l->width; x++)
        {
            if(x < 0 || x >= fb->size.width ||
               y < 0 || y >= fb->size.height)
                    continue;

            pixel = (fb->data + y*fb->size.width + x);
            *pixel = p->color;
        }
    }
}
