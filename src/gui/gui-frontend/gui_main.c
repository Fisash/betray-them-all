#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "gui/gui-frontend/gui_main.h"

#define START_WIDTH 800
#define START_HEIGHT 480

static void frame_buffer_init(frame_buffer_t *buffer, 
                               int width, int height)
{
    buffer->size.width = width;
    buffer->size.height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
}

static void frame_buffer_update(frame_buffer_t *buffer)
{
    int x, y;
    uint32_t *pixel;
    for(y = 0; y < buffer->size.height; y++)
    {
        for(x = 0; x < buffer->size.width; x++)
        {
            pixel = (buffer->data + y*buffer->size.width + x);
            *pixel = (rand() % UINT32_MAX);
        }
    }
}

static void gui_process_input(platform_interface_t *platform,
                window_id_t window_id, frame_buffer_t *frame)
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
                frame_buffer_update(frame);
                break;
            case INP_EVENT_KEY_DOWN:
                printf("key down: %d\n", event.values.keycode);
                break;
            case INP_EVENT_KEY_UP:
                printf("key up: %d\n", event.values.keycode);
                break;
            default:
                break;
        }
    }
}

void gui_run(platform_interface_t *platform)
{
    srand(1234);
    frame_buffer_t frame;
    frame_buffer_init(&frame, START_WIDTH, START_HEIGHT);

    window_id_t window_id = platform->create_window("gui-demo", 
                                    START_WIDTH, START_HEIGHT);

    for(;;)
    {
        gui_process_input(platform, window_id, &frame);
        platform->draw_frame(window_id, &frame);
    }
}

