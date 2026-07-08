#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "core/state/world.h"
#include "core/value_noise.h"

static void cell_list_init(cell_t **list)
{
    int i;
    for(i = 0; i < CELL_LIST_SIZE; i++)
        list[i] = NULL;
}

void world_cell_list_add(cell_t** list, cell_t *cell)
{
    int i;
    for(i = 0; i < CELL_LIST_SIZE; i++)
    {
        if(list[i] == NULL)
        {
            list[i] = cell;
            return;
        }
    }
}

static void cell_generate(int x, int y, world_t *world, uint32_t seed)
{
    double h = value_noise(x, y, seed);
    world->cells[y][x].type_id = 1 + (h > 0.5) + (h > 0.7);

    world->cells[y][x].days_until_update = 0;
}

void world_generate(world_t *world)
{
    srand(time(NULL));
    uint32_t seed = rand() % UINT32_MAX;
    int x, y;
    for (y = 0; y < WORLD_HEIGHT; y++)
    {
        for (x = 0; x < WORLD_WIDTH; x++)
            cell_generate(x, y, world, seed);
    }
    cell_list_init(world->event_cd_cells);
}

void world_fill_cells_id_buffer(world_t *world, char *buffer)
{
    int x, y;
    for (y = 0; y < WORLD_HEIGHT; y++)
    {
        for (x = 0; x < WORLD_WIDTH; x++)
            *(buffer++) = world->cells[y][x].type_id;
    }
}
