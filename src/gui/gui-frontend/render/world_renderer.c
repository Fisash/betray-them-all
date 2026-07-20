#include "gui/gui-frontend/render/world_renderer.h"

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

static uint32_t get_cell_view_pixel(cell_type_id_t type, int x, int y,
                                             const sprite_t sprites[])
{
    sprite_id_t sprite_id;
    switch(type)
    {
        case CELL_TYPE_MEADOW:   sprite_id = SPRITE_CELL_MEADOW;
        break;
        case CELL_TYPE_FOREST:   sprite_id = SPRITE_CELL_FOREST;
        break;
        case CELL_TYPE_MOUNTAIN: sprite_id = SPRITE_CELL_MOUNTAIN;
        break;
        case CELL_TYPE_VILLAGE:  sprite_id = SPRITE_CELL_VILLAGE;
        break;
        default:
            return 0x111111U;
    }

    return sprite_get_color(&sprites[sprite_id], x, y);
}

static uint32_t get_world_view_pixel(const world_t *world, 
                        const camera_t *cam, int x, int y,
                                 const sprite_t sprites[])
{
    int wx = x / cam->scale + cam->pos_x; 
    int wy = y / cam->scale + cam->pos_y;

    cell_type_id_t type = get_cell_type((cell_t*)world->cells, wx, wy);
    return get_cell_view_pixel(type, wx % CELL_WIDTH_PIXELS, 
                                     wy % CELL_HEIGHT_PIXELS,
                                                    sprites);
}

void world_renderer_render(frame_buffer_t *fb, const camera_t *cam, 
                                              const world_t *world,
                                          const sprite_t sprites[])
{
    int x, y;
    uint32_t *pixel;
    for(y = 0; y < fb->size.height; y++)
    {
        for(x = 0; x < fb->size.width; x++)
        {
            pixel = (fb->data + y*fb->size.width + x);
            *pixel = get_world_view_pixel(world, cam, x, y, sprites);
        }
    }
}
