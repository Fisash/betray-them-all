#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "gui/gui-frontend/gui_main.h"
#include "gui/gui-frontend/camera.h"
#include "gui/gui-frontend/layout_node.h"

#include "gui/gui-frontend/render/world_renderer.h"
#include "gui/gui-frontend/render/text_renderer.h"
#include "gui/gui-frontend/render/panel_renderer.h"

#include "gui/gui-frontend/res/game_resources.h"


#define START_WIDTH 800
#define START_HEIGHT 480

static struct game_resources resources;
static struct frame_buffer fb;
static struct camera cam;

static struct layout_node root;
static struct panel_renderer squad_panel;
static struct text_renderer text1;
static struct text_renderer text2;

static void ui_init()
{
    memset(&root, 0, sizeof(root));
    layout_node_update_root(&root, START_WIDTH, START_HEIGHT);

    layout_node_init(&squad_panel.layout, ALIGN_END,    0, 
                                          ALIGN_CENTER, 0,
                          SIZE_PARENT_NODE_PERCENT, 30.0f,  
                         SIZE_PARENT_NODE_PERCENT, 100.0f, 
                                                   &root);
    squad_panel.color = 0x808590U;
    
    layout_node_init(&text1.layout, ALIGN_CENTER, 0, 
                                    ALIGN_START, 10,
                                 SIZE_CUSTOM, 0.0f,  
                                 SIZE_CUSTOM, 0.0f, 
                              &squad_panel.layout);
    text_renderer_init(&text1, "hello hi", 
                       &resources.fonts[FONT_CONSOLE], 
                       2, 0, 0xFFFFFFU, 0x000000U);

    layout_node_init(&text2.layout, ALIGN_START,10, 
                                    ALIGN_END,  10,
                                 SIZE_CUSTOM, 0.0f,  
                                 SIZE_CUSTOM, 0.0f, 
                                            &root);
    text_renderer_init(&text2, "omg big\nand multiline", 
                       &resources.fonts[FONT_CONSOLE], 
                       2, 0, 0xFFFFFFU, 0x000000U);
}

static void layout_calc()
{
    layout_node_calc(&squad_panel.layout);
    text_renderer_calc_layout(&text1);
    text_renderer_calc_layout(&text2);
}

static void frame_buffer_init(struct frame_buffer *buffer, 
                                    int width, int height)
{
    buffer->size.width = width;
    buffer->size.height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
}

static void process_input_key_down(int keycode, struct camera *cam)
{
    switch(keycode)
    {
        case KEY_ESCAPE:
            exit(0);
        default:
            break;
    }
    camera_process_input_key_down(cam, keycode);
}

static void gui_process_input(struct platform_interface *platform,
                              window_id win, struct frame_buffer *frame,
                              struct camera *camera, const struct game_state *game)
{
    struct input_event event;
    for(;;)
    {
        platform->poll_event(win, &event);
        if(event.type ==  INP_EVENT_NONE)
            break;

        switch(event.type)
        {
            case INP_EVENT_WINDOW_RESIZE:
                struct window_size new_size = event.values.size;
                free(frame->data);
                frame_buffer_init(frame, new_size.width, new_size.height);
                layout_node_update_root(&root, new_size.width, 
                                             new_size.height);
                layout_calc();

                break;
            case INP_EVENT_KEY_DOWN:
                process_input_key_down(event.values.keycode, camera);
                break;
            case INP_EVENT_KEY_UP:
                break;
            default:
                break;
        }
    }
}

void gui_run(struct platform_interface *platform, struct game_state* game_state, 
                                              const struct game_info *game_info)
{
    game_resources_load(&resources);
    frame_buffer_init(&fb, START_WIDTH, START_HEIGHT);

    window_id win = platform->create_window("gui-demo", 
                            START_WIDTH, START_HEIGHT);

    camera_init(&cam);

    ui_init();

    for(;;)
    {
        gui_process_input(platform, win, 
                 &fb, &cam, game_state);

        world_renderer_render(&fb, &cam, &game_state->world, resources.sprites);
        panel_renderer_render(&squad_panel, &fb);
        text_renderer_render(&text1, &fb);
        text_renderer_render(&text2, &fb);

        platform->draw_frame(win, &fb);
    }
}
