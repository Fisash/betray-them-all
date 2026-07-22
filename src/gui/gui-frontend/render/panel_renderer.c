#include "gui/gui-frontend/render/panel_renderer.h"

void panel_renderer_render(struct panel_renderer *p, 
                            struct frame_buffer *fb)
{
    uint32_t *pixel;
    int x, y;
    struct layout_node_output *l = &p->layout.out;

    for(y = 0; y < fb->size.height; y++)
    {
        for(x = 0; x < fb->size.width; x++)
        {
            pixel = (fb->data + y*fb->size.width + x);
            if((x >= l->x) && (x < l->x + l->width) &&
               (y >= l->y) && (y < l->y + l->height))
                    *pixel = p->color;
        }
    }
}
