#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "gui/platform_interface.h"

#include "core/state/game_state.h"


void gui_run(platform_interface_t *platform, game_state_t* game_state, 
                                        const game_info_t *game_info);

#endif
