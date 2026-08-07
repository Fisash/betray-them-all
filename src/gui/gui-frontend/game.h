#ifndef GAME_H
#define GAME_H

#include "core/core.h"
#include "gui/platform.h"
#include "gui/gui-frontend/scenes/scene.h"

struct game {
    struct core core;

    struct frame_buffer fb;
    window_id window;
    struct game_resources *resources;

    struct scene active_scene;
    int is_running;
};

void game_init(struct game *game);
void game_run(struct game *game);
void game_destory(struct game *game);

#endif
