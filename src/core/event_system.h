#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "core/defs/events_info.h"
#include "core/defs/game_info.h"
#include "core/state/game_state.h"

void event_system_handle_answer(int answer_index, game_state_t *state, 
                                             const game_info_t *info);

int16_t 
event_system_choose_explore_event_id(const events_info_t *info, 
                                                 cell_t *cell);

const event_t *event_system_choose_explore_event(const events_info_t *info, 
                                                             cell_t *cell);

int16_t event_system_get_event_id(event_t *event, 
                       const events_info_t *info);
#endif
