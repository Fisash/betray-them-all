#ifndef PANEL_RENDERER_H
#define PANEL_RENDERER_H

#include <stdint.h>

#include "gui/platform_interface.h"
#include "gui/gui-frontend/layout_node.h"

struct panel_renderer {
    struct layout_node layout;
    uint32_t color;
};

void panel_renderer_render(struct panel_renderer *p, 
                           struct frame_buffer *fb);

#endif
