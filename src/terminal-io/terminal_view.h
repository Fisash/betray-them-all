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

#define CELL_VALUE1 '-'
#define CELL_VALUE2 '!'
#define CELL_VALUE3 '^'

#define SQUAD_CELL_VALUE 'p'

#define CELL_VALUE_DEFAULT '#'

typedef struct {
    world_t *world;
    squad_t *squad;
    const cell_info_t *cells_info;
    const item_info_t *items_info;
    int days;
} draw_frame_context_t;

void draw_context_init(draw_frame_context_t *context, world_t *world, 
                       squad_t *squad, const cell_info_t cells_info[], 
                             const item_info_t items_info[], int days);

void terminal_view_init_framebuffer(char* framebuffer_ptr);


void terminal_view_update_framebuffer(char *framebuffer, 
                           draw_frame_context_t *context);

void terminal_view_redraw(char *framebuffer, 
               draw_frame_context_t *context);

void terminal_view_stdout_framebuffer(char *framebuffer);

#endif
