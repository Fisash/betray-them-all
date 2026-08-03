#include <stdlib.h>

#include "gui/frame_buffer.h"

void frame_buffer_init(struct frame_buffer *fb, 
                         int width, int height)
{
    fb->size.width = width;
    fb->size.height = height;
    fb->data = malloc(width * height * sizeof(uint32_t));
}
