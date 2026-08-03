#ifndef SCENE_H
#define SCENE_H

#include "gui/gui-frontend/render/renderer.h"
#include "gui/gui-frontend/res/game_resources.h"
#include "core/core.h"

#define RENDERERS_MAX 16

enum scene_id {
    SCENE_NONE,
    SCENE_MAIN
};

struct scene {
    enum scene_id id;
    struct layout_node root;
    struct renderer renderers[RENDERERS_MAX];
};

void scene_load(struct scene *s,  enum scene_id id, 
    struct game_resources *res, struct core *core);

void scene_update_layout(struct scene *s);

void scene_process_ie(struct scene *s, struct input_event *ie);

void scene_draw(struct scene *s, struct frame_buffer *fb);

#endif
