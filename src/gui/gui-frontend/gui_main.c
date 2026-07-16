#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "gui/gui-frontend/gui_main.h"
#include "gui/gui-frontend/sprites.h"

#define START_WIDTH 800
#define START_HEIGHT 480

static void frame_buffer_init(frame_buffer_t *buffer, 
                               int width, int height)
{
    buffer->size.width = width;
    buffer->size.height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
}

static cell_type_id_t get_cell_type(const cell_t *world_cells, 
                                                 int x, int y)
{
    int x_cell = x/CELL_WIDTH_PIXELS;
    int y_cell = y/CELL_HEIGHT_PIXELS;
    if(x_cell < 0 || x_cell > WORLD_WIDTH ||
       y_cell < 0 || y_cell > WORLD_HEIGHT)
            return CELL_TYPE_NONE;

    const cell_t *cell = world_cells + WORLD_WIDTH * y_cell + x_cell;
    return cell->type_id;
}

static void fill_cell_view_pixel(cell_type_id_t type, uint32_t *pixel,
                                                         int x, int y)
{
    switch(type)
    {
        case CELL_TYPE_MEADOW:
            *pixel = cell_meadow_sprite[y][x];
            return;
        case CELL_TYPE_FOREST:
            *pixel = cell_forest_sprite[y][x];
            return;
        case CELL_TYPE_MOUNTAIN:
            *pixel = cell_mountain_sprite[y][x];
            return;
        case CELL_TYPE_VILLAGE:
            *pixel = cell_village_sprite[y][x];
            return;
        default:
            *pixel = 0x111111U;

    }
}

static void fill_world_view_pixel(const cell_t *world_cells, 
                               uint32_t *pixel,int x, int y, 
                                     const camera_t *camera)
{
    int wx = x / camera->scale;
    int wy = y / camera->scale;

    cell_type_id_t type = get_cell_type(world_cells, wx, wy);
    fill_cell_view_pixel(type, pixel, wx%CELL_WIDTH_PIXELS, 
                                    wy%CELL_HEIGHT_PIXELS);
}

static void frame_buffer_update(frame_buffer_t *buffer, 
                                const camera_t *camera, 
                              const game_state_t *game)
{
    int x, y;
    uint32_t *pixel;
    for(y = 0; y < buffer->size.height; y++)
    {
        for(x = 0; x < buffer->size.width; x++)
        {
            pixel = (buffer->data + y*buffer->size.width + x);
            fill_world_view_pixel((cell_t*)game->world.cells, 
                                        pixel, x, y, camera);
        }
    }
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
                frame_buffer_update(frame, camera, game);
                break;
            case INP_EVENT_KEY_DOWN:
                if(event.values.keycode == KEY_ESCAPE)
                    exit(0);
                if(event.values.keycode == KEY_Z)
                {
                    camera->scale *= 2;
                    if(camera->scale >= 8) camera->scale = 8;
                    frame_buffer_update(frame, camera, game);
                }
                else if(event.values.keycode == KEY_X)
                {
                    camera->scale /= 2;
                    if(camera->scale < 1) camera->scale = 1;
                    frame_buffer_update(frame, camera, game);
                }
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
    frame_buffer_t frame;
    frame_buffer_init(&frame, START_WIDTH, START_HEIGHT);

    window_id_t window_id = platform->create_window("gui-demo", 
                                    START_WIDTH, START_HEIGHT);

    camera_t camera = {0};
    camera.scale = 2;

    for(;;)
    {
        gui_process_input(platform, window_id, 
                 &frame, &camera, game_state);
        platform->draw_frame(window_id, &frame);
    }
}

