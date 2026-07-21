#include "core/world_queries.h"

struct cell *world_queries_get_squad_cell(struct squad *squad, struct world *world)
{
    uint16_t pos_x = squad->pos_x;
    uint16_t pos_y = squad->pos_y;
    return &(world->cells[pos_y][pos_x]);
}
