#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "core/state/world.h"
#include "gui/platform_interface.h"

#include "gui/gui-frontend/res/sprites.h"
#include "gui/gui-frontend/camera.h"
#include "gui/gui-frontend/layout_node.h"

#define CELL_WIDTH_PIXELS 16
#define CELL_HEIGHT_PIXELS 16

struct world_renderer {
    struct layout_node layout;

    const struct camera *cam;
    const struct world *world;
    const struct sprite *sprites;
};

void world_renderer_init(struct world_renderer *r, 
                         const struct camera *cam, 
                         const struct world *world, 
                         const struct sprite *sprites);

void world_renderer_render(struct world_renderer *r, 
                           struct frame_buffer *fb);

#endif
