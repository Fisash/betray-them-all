#include <string.h>

#include "gui/gui-frontend/scenes/scene.h"

#include "gui/gui-frontend/scenes/main.h"


void scene_load(struct scene *s, enum scene_id id, 
    struct game_resources *res, struct core *core)
{
    s->id = id;
    memset(&s->root, 0, sizeof(s->root));
    switch(s->id)
    {
        case SCENE_MAIN:
            scene_main_load(s, res, core);
            break;
        default:
            break;
    }
}

void scene_update_layout(struct scene *s)
{
    struct renderer *r;
    int i;
    for(i = 0; i < RENDERERS_MAX; i++)
    {
        r = &s->renderers[i];
        if(r->type == RENDERER_NONE)
            continue;
        renderer_update_layout(r);
    }
}

void scene_process_ie(struct scene *s, struct input_event *ie)
{
    switch(s->id)
    {
        case SCENE_MAIN:
            scene_main_process_ie(s, ie);
            break;
         default:
            break;
    }
}

void scene_draw(struct scene *s, struct frame_buffer *fb)
{
    struct renderer *r;
    int i;
    for(i = 0; i < RENDERERS_MAX; i++)
    {
        r = &s->renderers[i];
        if(r->type == RENDERER_NONE)
            continue;
        renderer_render(r, fb);
    }
}
