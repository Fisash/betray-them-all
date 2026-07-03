#include <stdio.h>
#include <stdlib.h>

#include "core/event_system.h"

static void start_forest_animals_battle(battle_state_t *b, 
                    squad_t *squad, const game_info_t *info)
{
    battle_state_init(b, squad);

    unit_t wolf1;
    unit_init(&wolf1, "Wolf", WOLF, info->unit_templates, info->items);
    battle_state_add_enemy(b, &wolf1);
}

void event_system_handle_answer(event_answer_handle_context_t c)
{
    const event_answer_t *answer = &c.event->answers[c.answer_index];
    switch(answer->action_id)
    {
        case ACTION_START_ANIMALS_BATTLE:
            start_forest_animals_battle(c.battle_state, c.squad, c.info);
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

    uint8_t type = cell->type_id;
    uint64_t type_tag_mask = (uint64_t)1<<type;

    uint16_t weight_sum = info->cell_type_weight_sums[type];

    if(weight_sum == 0)
        return EVENT_NONE;

    uint16_t rand_value = rand() % weight_sum;
    uint16_t weight_cumulative = 0;
    const event_t *current_event;
    for(uint16_t i = 0; i < EVENT_COUNT; i++)
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
    for(int16_t i = 0; i < EVENT_COUNT; i++)
    {
        if(event == &info->events[i])
            return i;
    }

    return EVENT_NONE;
}
