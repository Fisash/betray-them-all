#include "gui/gui-frontend/render/world_renderer.h"

void world_renderer_init(struct world_renderer *r, 
                         const struct camera *cam, 
                         const struct world *world, 
                         const struct sprite *sprites)
{
    r->cam = cam;
    r->world = world;
    r->sprites = sprites;
}

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

static uint32_t get_world_view_pixel(struct world_renderer *r, 
                                                 int x, int y)
{
    int wx = x / r->cam->scale + r->cam->pos_x; 
    int wy = y / r->cam->scale + r->cam->pos_y;

    enum cell_type_id type = 
        get_cell_type((struct cell*)r->world->cells, wx, wy);
    return get_cell_view_pixel(type, wx %  CELL_WIDTH_PIXELS, 
                                     wy % CELL_HEIGHT_PIXELS,
                                                 r->sprites);
}

void world_renderer_render(struct world_renderer *r,
                           struct frame_buffer *fb)
{
    int x, y;
    uint32_t *pixel;
    struct layout_node_output *l = &r->layout.out;
    for(y = l->y; y < l->y + l->height; y++)
    {
        for(x = l->x; x < l->x + l->width; x++)
        {
            if(x < 0 || x >= fb->size.width ||
               y < 0 || y >= fb->size.height)
                    continue;

            pixel = (fb->data + y*fb->size.width + x);
            *pixel = get_world_view_pixel(r, x, y);
        }
    }
}
