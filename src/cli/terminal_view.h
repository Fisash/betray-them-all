#ifndef TERMINAL_VIEW_H
#define TERMINAL_VIEW_H

#include "core/state/world.h"
#include "core/state/squad.h"
#include "core/defs/cell_info.h"
#include "core/defs/items_info.h"

#define FRAME_WIDTH 80
#define FRAME_HEIGHT 25

#define WORLD_OFFSET_X 1
#define WORLD_OFFSET_Y 1

#define CELL_VIEW_MEADOW '-'
#define CELL_VIEW_FOREST '!'
#define CELL_VIEW_MOUNTAIN '^'
#define CELL_VIEW_VILLAGE 'v'
#define CELL_VIEW_SQUAD 'p'

#define CELL_VIEW_DEFAULT '#'

struct draw_frame_context {
    struct world *world;
    struct squad *squad;
    const struct cell_info *cells_info;
    const struct item_info *items_info;
    int days;
};

void draw_context_init(struct draw_frame_context *context, struct world *world, 
                           struct squad *squad, const struct cell_info cells_info[], 
                                const struct item_info items_info[], int days);

void terminal_view_init_framebuffer(char* framebuffer_ptr);


void terminal_view_update_framebuffer(char *framebuffer, 
                                      struct draw_frame_context *context);

void terminal_view_redraw(char *framebuffer, 
                          struct draw_frame_context *context);

void terminal_view_stdout_framebuffer(char *framebuffer);

#endif
