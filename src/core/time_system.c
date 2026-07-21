#include "core/time_system.h"

#include <stddef.h>
#include "core/squad_movement.h"

static void tick_world_cooldowns(struct cell **cd_list, uint16_t days)
{
    struct cell *cell;
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

void time_system_spend(struct game_state *state, uint16_t days)
{
    tick_world_cooldowns(state->world.event_cd_cells, days);

    while (days > 0)
    {
        state->days++;
        days--;
        movement_try_move_squad_by_order(&state->squad);

        squad_consume_day_provision(&state->squad);
        if(state->squad.provision == 0)
            state->is_over = 1;
    }
}
