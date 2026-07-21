#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "gui/gui-frontend/gui_main.h"
#include "gui/gui-frontend/camera.h"

#include "gui/gui-frontend/render/world_renderer.h"
#include "gui/gui-frontend/render/text_renderer.h"

#include "gui/gui-frontend/res/game_resources.h"


#define START_WIDTH 800
#define START_HEIGHT 480

static void frame_buffer_init(frame_buffer_t *buffer, 
                               int width, int height)
{
    buffer->size.width = width;
    buffer->size.height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
}

static void process_input_key_down(int keycode, camera_t *cam)
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

static void gui_process_input(platform_interface_t *platform,
                window_id_t window_id, frame_buffer_t *frame,
                  camera_t *camera, const game_state_t *game)
{
    input_event_t event;
    for(;;)
    {
        platform->poll_event(window_id, &event);
        if(event.type ==  INP_EVENT_NONE)
            break;

        switch(event.type)
        {
            case INP_EVENT_WINDOW_RESIZE:
                window_size_t new_size = event.values.size;
                free(frame->data);
                frame_buffer_init(frame, new_size.width, new_size.height);
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


void gui_run(platform_interface_t *platform, game_state_t* game_state, 
                                         const game_info_t *game_info)
{
    game_resources_t resources;
    game_resources_load(&resources);

    frame_buffer_t fb;
    frame_buffer_init(&fb, START_WIDTH, START_HEIGHT);

    window_id_t win = platform->create_window("gui-demo", 
                              START_WIDTH, START_HEIGHT);

    camera_t cam;
    camera_init(&cam);

    text_renderer_t text_renderer;
    text_renderer_init(&text_renderer, &fb, 
                       &resources.fonts[FONT_CONSOLE], 
                       2, 0, 0xFFFFFFU, 0x000000U);

    for(;;)
    {
        gui_process_input(platform, win, 
                 &fb, &cam, game_state);
        world_renderer_render(&fb, &cam, &game_state->world, resources.sprites);

        text_renderer_render(&text_renderer, 
            "lmao hehehe\nwaaaa", 0, 0);
        platform->draw_frame(win, &fb);
    }
}
