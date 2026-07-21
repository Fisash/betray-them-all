#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "gui/platform_interface.h"

#include "core/state/game_state.h"


void gui_run(struct platform_interface *platform, struct game_state* game_state, 
                                              const struct game_info *game_info);

#endif
