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
    struct game_resources resources;
    game_resources_load(&resources);

    struct frame_buffer fb;
    frame_buffer_init(&fb, START_WIDTH, START_HEIGHT);

    window_id win = platform->create_window("gui-demo", 
                              START_WIDTH, START_HEIGHT);

    struct camera cam;
    camera_init(&cam);

    struct text_renderer text;
    text_renderer_init(&text, &fb, 
                       &resources.fonts[FONT_CONSOLE], 
                       2, 0, 0xFFFFFFU, 0x000000U);

    for(;;)
    {
        gui_process_input(platform, win, 
                 &fb, &cam, game_state);
        world_renderer_render(&fb, &cam, &game_state->world, resources.sprites);

        text_renderer_render(&text, "lmao hehehe\nwaaaa", 0, 0);

        platform->draw_frame(win, &fb);
    }
}
