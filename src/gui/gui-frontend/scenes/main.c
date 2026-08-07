#include <stdio.h>

#include "gui/gui-frontend/scenes/main.h"

#include "gui/gui-frontend/camera.h"

static struct camera cam;

static struct world_renderer world_view;
static struct panel_renderer squad_panel;
static struct text_renderer text1;
static struct text_renderer text2;

enum {
    WORLD_VIEW,
    SQUAD_PANEL,
    TEXT1,
    TEXT2
};

void scene_main_load(struct scene *s, struct game_resources *res, 
                                               struct core *core)
{
    camera_init(&cam);
    /* ----------------------------------------------------------- */
    layout_node_init(&world_view.layout, ALIGN_START,  0, 
                                         ALIGN_CENTER, 0,
                         SIZE_PARENT_NODE_PERCENT, 65.0f,  
                        SIZE_PARENT_NODE_PERCENT,  90.0f, 
                                               &s->root);
    world_renderer_init(&world_view, &cam, &core->state->world,
                                                 res->sprites);
    renderer_init_world(&s->renderers[WORLD_VIEW], &world_view);
    /* ----------------------------------------------------------- */
    layout_node_init(&squad_panel.layout, ALIGN_END,    0, 
                                          ALIGN_CENTER, 0,
                          SIZE_PARENT_NODE_PERCENT, 30.0f,  
                         SIZE_PARENT_NODE_PERCENT, 100.0f, 
                                                &s->root);
    squad_panel.color = 0x808590U;
    renderer_init_panel(&s->renderers[SQUAD_PANEL], &squad_panel);
    
    /* ----------------------------------------------------------- */
    layout_node_init(&text1.layout, ALIGN_CENTER, 0, 
                                    ALIGN_START, 10,
                                 SIZE_CUSTOM, 0.0f,  
                                 SIZE_CUSTOM, 0.0f, 
                              &squad_panel.layout);
    text_renderer_init(&text1, "hello hi", 
                &res->fonts[FONT_CONSOLE], 
              2, 0, 0xFFFFFFU, 0x000000U);
    renderer_init_text(&s->renderers[TEXT1], &text1);

    /* ----------------------------------------------------------- */
    layout_node_init(&text2.layout, ALIGN_START,10, 
                                    ALIGN_END,  10,
                                 SIZE_CUSTOM, 0.0f,  
                                 SIZE_CUSTOM, 0.0f, 
                                            &s->root);
    text_renderer_init(&text2, "omg big\nand multiline", 
                              &res->fonts[FONT_CONSOLE], 
                            2, 0, 0xFFFFFFU, 0x000000U);
    renderer_init_text(&s->renderers[TEXT2], &text2);
    /* ----------------------------------------------------------- */
}

static void process_input_key_down(struct scene *s, int keycode)
{
    switch(keycode)
    {
        case KEY_ESCAPE:
            scene_request_exit(s, 0);
        default:
            break;
    }
    camera_process_input_key_down(&cam, keycode);
}

void scene_main_process_ie(struct scene *s, struct input_event *ie)
{
    switch(ie->type)
    {
        case INP_EVENT_KEY_DOWN:
            process_input_key_down(s, ie->values.keycode);
            break;
        default:
            break;
    }
}
