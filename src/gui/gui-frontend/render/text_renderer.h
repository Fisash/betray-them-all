#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gui/platform_interface.h"

#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8

void text_renderer_render(frame_buffer_t *fb, const char *rendering, 
                              int x_offset, int y_offset, int scale, 
                                                 int is_background);
#endif
