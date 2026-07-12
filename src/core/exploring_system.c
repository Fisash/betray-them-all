#include "core/exploring_system.h"

#include "core/event_system.h"
#include "core/world_queries.h"

void exploring_system_explore_cell(cell_t* cell, cell_t **cd_list,
                        const events_info_t *info, int16_t *active,
                                    const cell_info_t cells_info[])
{
    const cell_info_t *cell_info = &cells_info[cell->type_id];

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

void exploring_system_explore_squad_cell(squad_t *squad, world_t *world,
                              const events_info_t *info, int16_t *active,
                                          const cell_info_t cells_info[])
{
    cell_t *cell = world_queries_get_squad_cell(squad, world);
    exploring_system_explore_cell(cell, world->event_cd_cells, 
                                    info, active, cells_info);
}
