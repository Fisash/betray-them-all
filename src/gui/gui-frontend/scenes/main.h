#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "gui/gui-frontend/scenes/scene.h"

void scene_main_load(struct scene *s, struct game_resources *res, 
                                              struct core *core);

void scene_main_process_ie(struct scene *s, struct input_event *ie);

#endif
