#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "core/state/battle_state.h"

/* PROTOCOL */
/* next_turn    ->  get_skill_context -> execute_action -> ...*/
/* turn_context ->  skill_use_context -> action_report  -> ...*/

battle_turn_context_t battle_system_next_turn(battle_state_t *state, 
                                         const skill_t all_skills[],
                                  const unit_template_t templates[]);
/* how to get available skills: 
 * get learned skills + filter by weapon group + filter by props req */

#endif
