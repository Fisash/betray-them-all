#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#include <stdint.h>

typedef struct {
    int width;
    int height;
} window_size_t;

typedef struct {
    window_size_t size;
    uint32_t *data;
} frame_buffer_t;

typedef enum {
    INP_EVENT_NONE,
    INP_EVENT_EXPOSE,
    INP_EVENT_WINDOW_RESIZE,
    INP_EVENT_KEY_DOWN,
    INP_EVENT_KEY_UP
} input_event_type_t;

typedef struct {
    input_event_type_t type;
    union {
        int keycode;
        window_size_t size;
    } values;
} input_event_t;

typedef void* window_id_t;

typedef struct {
    window_id_t (*create_window)(const char*, int, int);
    void (*destroy_window)(window_id_t);
    void (*draw_frame)(window_id_t, frame_buffer_t*);
    void (*poll_event)(window_id_t, input_event_t*);
} platform_interface_t;

#endif
