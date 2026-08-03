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

struct platform_interface {
    window_id (*create_window)(const char *, int, int);
    void (*destroy_window)(window_id);
    void (*draw_frame)(window_id, struct frame_buffer *);
    void (*poll_event)(window_id, struct input_event *);
};

#endif
