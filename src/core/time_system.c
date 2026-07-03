#include "core/time_system.h"

#include <stddef.h>
#include "core/squad_movement.h"

static void tick_world_cooldowns(cell_t **cd_list, uint16_t days)
{
    cell_t *cell;
    int i;
    for (i = 0; i < CELL_LIST_SIZE; i++)
    {
        cell = cd_list[i];
        if(cell == NULL)
            continue;

        if (cell->days_until_update > days)
            cell->days_until_update -= days;
        else
        {
            cell->days_until_update = 0;
            cd_list[i] = NULL;
        }
    }
}

void time_system_spend(squad_t *squad, world_t *world, uint16_t days, 
                                                uint16_t *days_state)
{
    tick_world_cooldowns(world->event_cd_cells, days);

    while (days > 0)
    {
        *days_state += days--;
        movement_try_move_squad_by_order(squad);
    }
}
