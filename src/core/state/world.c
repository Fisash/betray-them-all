#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "core/state/world.h"
#include "core/state/item_storage.h"
#include "core/value_noise.h"

void world_cell_list_add(struct cell** list, struct cell *cell)
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

static void cell_generate_terrain(int x, int y, struct world *world, 
                                                 uint32_t seed)
{
    enum cell_type_id id;
    double h = value_noise(x, y, seed);

    if(h > 0.7)
        id = CELL_TYPE_MOUNTAIN;
    else if (h > 0.5)
        id = CELL_TYPE_FOREST;
    else
        id = CELL_TYPE_MEADOW;

    world->cells[y][x].type_id = id;
    world->cells[y][x].days_until_update = 0;
}

static void cell_generate_village(struct world *world, const struct item_info items[])
{
    struct cell *cell;
    int x = rand() % WORLD_WIDTH;
    int y = rand() % WORLD_HEIGHT;

    cell = &(world->cells[y][x]);
    if(cell->type_id == CELL_TYPE_VILLAGE)
    {
        cell_generate_village(world, items);
        return;
    }

    cell->type_id = CELL_TYPE_VILLAGE;

    cell->data_index = world->village_count++;
    struct village *village = &(world->villages[cell->data_index]);

    shop_init(&village->shop, 100, 1.5f, 1.0f);
    shop_generate_village_items(&village->shop, items);
}

/* todo: reading user`s input seed, not only random generated seed*/
static uint32_t get_world_seed()
{
    srand(time(NULL));
    return rand() % UINT32_MAX;
}

void world_generate(struct world *world, const struct item_info items[])
{
    uint32_t seed = get_world_seed();
    srand(seed);

    memset(world, 0, sizeof(struct world));

    int x, y, i;
    for (y = 0; y < WORLD_HEIGHT; y++)
    {
        for (x = 0; x < WORLD_WIDTH; x++)
            cell_generate_terrain(x, y, world, seed);
    }
    
    for(i = 0; i < VILLAGE_MAX_COUNT; i++)
        cell_generate_village(world, items);
}

void world_fill_cells_id_buffer(struct world *world, char *buffer)
{
    int x, y;
    for (y = 0; y < WORLD_HEIGHT; y++)
    {
        for (x = 0; x < WORLD_WIDTH; x++)
            *(buffer++) = (char)world->cells[y][x].type_id;
    }
}
