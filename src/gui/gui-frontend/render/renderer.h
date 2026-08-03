#ifndef RENDERER_H
#define RENDERER_H

#include "gui/gui-frontend/render/panel_renderer.h"
#include "gui/gui-frontend/render/text_renderer.h"
#include "gui/gui-frontend/render/world_renderer.h"
#include "gui/frame_buffer.h"

enum renderer_type {
    RENDERER_NONE,
    RENDERER_PANEL,
    RENDERER_TEXT,
    RENDERER_WORLD
};

struct renderer {
    enum renderer_type type;
    union {
        struct panel_renderer *panel;
        struct text_renderer *text;
        struct world_renderer *world;
    } props;
};

void renderer_render(struct renderer *r, struct frame_buffer *fb);

void renderer_update_layout(struct renderer *r);

void renderer_init_panel(struct renderer *r, 
                         struct panel_renderer *panel);
void renderer_init_text(struct renderer *r, 
                        struct text_renderer *text);
void renderer_init_world(struct renderer *r, 
                         struct world_renderer *world);

#endif
