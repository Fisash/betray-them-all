#include <stdio.h>
#include <stdlib.h>

#include "core/event_system.h"
#include "core/world_queries.h"

static void start_forest_animals_battle(battle_state_t *b, 
                    squad_t *squad, const game_info_t *info)
{
    battle_state_init(b, squad, info->skills, info->items, 
                                    info->unit_templates);

    unit_t wolf1;
    unit_init(&wolf1, "Wolf", WOLF, info->unit_templates, info->items);
    battle_state_add_enemy(b, &wolf1);
}

static void open_village_shop(shop_t **active_shop, 
                    squad_t *squad, world_t *world)
{
    cell_t *squad_cell = world_queries_get_squad_cell(squad, world);
    if(squad_cell->type_id != CELL_TYPE_VILLAGE)
        return;
    *active_shop = &(world->villages[squad_cell->data_index].shop);
}

void event_system_handle_answer(int answer_index, game_state_t *state, 
                                              const game_info_t *info)
{
    const event_t *active = 
                  &info->events_info.events[state->active_event_id];
    const event_answer_t *answer = &(active->answers[answer_index]);
    switch(answer->action_id)
    {
        case ACTION_OPEN_VILLAGE_SHOP:
            open_village_shop(&state->active_shop, &state->squad, 
                                                   &state->world);
            break;
        case ACTION_START_ANIMALS_BATTLE:
            start_forest_animals_battle(&state->battle, 
                                  &state->squad, info);
            break;
        case ACTION_NONE:
        default:
            break; 
    }
}

int16_t event_system_choose_explore_event_id(const events_info_t *info, 
                                                          cell_t *cell)
{
    if(cell->days_until_update > 0)
        return EVENT_EXPLORED;

    int16_t weight_sum, rand_value, weight_cumulative, i;

    uint8_t type = cell->type_id;
    uint64_t type_tag_mask = (uint64_t)1<<type;

    weight_sum = info->cell_type_weight_sums[type];

    if(weight_sum == 0)
        return EVENT_NONE;

    const event_t *current_event;
    rand_value = rand() % weight_sum;
    weight_cumulative = 0;
    for(i = 0; i < EVENT_COUNT; i++)
    {
        current_event = &info->events[i];
        if((current_event->tags & type_tag_mask) == 0)
            continue;

        weight_cumulative += current_event->weight;
        if(rand_value < weight_cumulative)
            return i;
    }

    return EVENT_NONE;
}


const event_t *event_system_choose_explore_event(const events_info_t *info, 
                                                             cell_t *cell)
{
    int16_t id = event_system_choose_explore_event_id(info, cell);
    return &(info->events[id]);
}

int16_t event_system_get_event_id(event_t *event, 
                        const events_info_t *info)
{
    uint16_t i;
    for(i = 0; i < EVENT_COUNT; i++)
    {
        if(event == &info->events[i])
            return i;
    }

    return EVENT_NONE;
}
