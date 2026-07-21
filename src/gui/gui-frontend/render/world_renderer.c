#include "gui/gui-frontend/render/world_renderer.h"

static enum cell_type_id get_cell_type(const struct cell *world_cells, 
                                                         int x, int y)
{
    int x_cell = x/CELL_WIDTH_PIXELS;
    int y_cell = y/CELL_HEIGHT_PIXELS;
    if(x_cell < 0 || x_cell > WORLD_WIDTH ||
       y_cell < 0 || y_cell > WORLD_HEIGHT)
            return CELL_TYPE_NONE;

    const struct cell *cell = world_cells + WORLD_WIDTH * y_cell + x_cell;
    return cell->type_id;
}

static uint32_t get_cell_view_pixel(enum cell_type_id type, int x, int y,
                                           const struct sprite sprites[])
{
    enum sprite_id id;
    switch(type)
    {
        case CELL_TYPE_MEADOW:   id = SPRITE_CELL_MEADOW;
        break;
        case CELL_TYPE_FOREST:   id = SPRITE_CELL_FOREST;
        break;
        case CELL_TYPE_MOUNTAIN: id = SPRITE_CELL_MOUNTAIN;
        break;
        case CELL_TYPE_VILLAGE:  id = SPRITE_CELL_VILLAGE;
        break;
        default:
            return 0x111111U;
    }

    return sprite_get_color(&sprites[id], x, y);
}

static uint32_t get_world_view_pixel(const struct world *world, 
                                     const struct camera *cam, int x, int y,
                                              const struct sprite sprites[])
{
    int wx = x / cam->scale + cam->pos_x; 
    int wy = y / cam->scale + cam->pos_y;

    enum cell_type_id type = get_cell_type((struct cell*)world->cells, wx, wy);
    return get_cell_view_pixel(type, wx % CELL_WIDTH_PIXELS, 
                                     wy % CELL_HEIGHT_PIXELS,
                                                    sprites);
}

void world_renderer_render(struct frame_buffer *fb, const struct camera *cam,
                                                        const struct world *world,
                                               const struct sprite sprites[])
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
