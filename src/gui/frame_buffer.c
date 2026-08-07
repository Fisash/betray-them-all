#include <stdlib.h>
#include <string.h>

#include "gui/frame_buffer.h"

void frame_buffer_init(struct frame_buffer *fb, int width, int height)
{
    fb->size.width = width;
    fb->size.height = height;
    fb->data = calloc(width * height, sizeof(uint32_t));
}
