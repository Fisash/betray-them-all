#include "core/exploring_system.h"

#include "core/event_system.h"

void exploring_system_explore_cell(struct cell *cell, struct cell **cd_list,
                        const struct events_info *info, int16_t *active,
                                    const struct cell_info cells_info[])
{
    const struct cell_info *cell_info = &cells_info[cell->type_id];

    int16_t event_id =
    event_system_choose_explore_event_id(info, cell);

    if(event_id == EVENT_NONE)
        return;

    if(event_id != EVENT_EXPLORED && cell_info->exploring_cd > 0)
    {
        cell->days_until_update = cell_info->exploring_cd;
        world_cell_list_add(cd_list, cell);
    }

    *active = event_id;
}

void exploring_system_explore_squad_cell(struct squad *squad, struct world *world,
                              const struct events_info *info, int16_t *active,
                                          const struct cell_info cells_info[])
{
    struct cell *cell = &world->cells[squad->pos_y][squad->pos_x];
    exploring_system_explore_cell(cell, world->event_cd_cells, 
                                    info, active, cells_info);
}
