#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <stdint.h>

struct window_size {
    int width;
    int height;
};

struct frame_buffer {
    struct window_size size;
    uint32_t *data;
};

void frame_buffer_init(struct frame_buffer *fb, 
                        int width, int height);

#endif
