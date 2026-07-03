#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "core/defs/events_info.h"
#include "core/defs/game_info.h"
#include "core/state/world.h"
#include "core/state/battle_state.h"

typedef struct {
    const event_t *event;
    int answer_index;
    battle_state_t *battle_state;
    squad_t *squad;
    const game_info_t *info;
} event_answer_handle_context_t;

void event_system_handle_answer(event_answer_handle_context_t c);

int16_t 
event_system_choose_explore_event_id(const events_info_t *info, 
                                                 cell_t *cell);

const event_t *event_system_choose_explore_event(const events_info_t *info, 
                                                             cell_t *cell);

int16_t event_system_get_event_id(event_t *event, 
                       const events_info_t *info);
#endif
