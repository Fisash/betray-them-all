#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#include <stdint.h>
#include "gui/frame_buffer.h"

#define START_WIDTH 800
#define START_HEIGHT 480

#define KEY_ESCAPE 9

#define KEY_H 43
#define KEY_J 44
#define KEY_K 45
#define KEY_L 46

#define KEY_Z 52
#define KEY_X 53

enum input_event_type {
    INP_EVENT_NONE,
    INP_EVENT_EXPOSE,
    INP_EVENT_WINDOW_RESIZE,
    INP_EVENT_KEY_DOWN,
    INP_EVENT_KEY_UP
};

struct input_event {
    enum input_event_type type;
    union {
        int keycode;
        struct window_size size;
    } values;
};

typedef void* window_id;

window_id platform_create_window(const char *title, int width, int height);
void platform_destroy_window(window_id win);
void platform_draw_frame(window_id win, struct frame_buffer *fb);
void platform_poll_event(window_id win, struct input_event *event);

#endif
