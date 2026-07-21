#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "core/state/world.h"
#include "gui/platform_interface.h"

#include "gui/gui-frontend/res/sprites.h"
#include "gui/gui-frontend/camera.h"

#define CELL_WIDTH_PIXELS 16
#define CELL_HEIGHT_PIXELS 16

void world_renderer_render(struct frame_buffer *fb, const struct camera *cam, 
                                                   const struct world *world,
                                               const struct sprite sprites[]);

#endif
