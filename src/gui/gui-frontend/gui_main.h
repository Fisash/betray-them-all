#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "gui/platform_interface.h"

#include "core/state/game_state.h"

#define CELL_WIDTH_PIXELS 16
#define CELL_HEIGHT_PIXELS 16

typedef struct {
    int pos_x;
    int pos_y;
    int scale;
} camera_t;

void gui_run(platform_interface_t *platform, game_state_t* game_state, 
                                        const game_info_t *game_info);

#endif
