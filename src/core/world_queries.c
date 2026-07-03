#include "core/world_queries.h"

cell_t *world_queries_get_squad_cell(squad_t *squad, world_t *world)
{
    uint16_t pos_x = squad->pos_x;
    uint16_t pos_y = squad->pos_y;
    return &(world->cells[pos_y][pos_x]);
}
