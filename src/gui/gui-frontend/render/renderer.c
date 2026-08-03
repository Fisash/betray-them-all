#include "gui/gui-frontend/render/renderer.h"

void renderer_render(struct renderer *r, struct frame_buffer *fb)
{
    switch(r->type)
    {
        case RENDERER_PANEL:
            panel_renderer_render(r->props.panel, fb);
            break;
        case RENDERER_TEXT:
            text_renderer_render(r->props.text, fb);
            break;
        case RENDERER_WORLD:
            world_renderer_render(r->props.world, fb);
            break;
        default:
            break;
    }
}

void renderer_update_layout(struct renderer *r)
{
    switch(r->type)
    {
        case RENDERER_PANEL:
            layout_node_calc(&r->props.panel->layout);
            break;
        case RENDERER_TEXT:
            text_renderer_calc_layout(r->props.text);
            break;
        case RENDERER_WORLD:
            layout_node_calc(&r->props.world->layout);
            break;
        default:
            break;
    }
}

void renderer_init_panel(struct renderer *r, 
                         struct panel_renderer *panel)
{
    r->type = RENDERER_PANEL;
    r->props.panel = panel;
}

void renderer_init_text(struct renderer *r, 
                        struct text_renderer *text)
{
    r->type = RENDERER_TEXT;
    r->props.text = text;
}

void renderer_init_world(struct renderer *r, 
                         struct world_renderer *world)
{
    r->type = RENDERER_WORLD;
    r->props.world = world;
}
