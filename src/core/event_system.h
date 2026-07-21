#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "core/defs/events_info.h"
#include "core/defs/game_info.h"
#include "core/state/game_state.h"

void event_system_handle_answer(int answer_index, struct game_state *state, 
                                                   const struct game_info *info);

int16_t 
event_system_choose_explore_event_id(const struct events_info *info, 
                                                 struct cell *cell);

const struct event *event_system_choose_explore_event(const struct events_info *info,
                                                         struct cell *cell);

int16_t event_system_get_event_id(struct event *event, 
                                  const struct events_info *info);
#endif
