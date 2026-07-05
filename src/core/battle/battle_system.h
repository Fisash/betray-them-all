#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "core/state/battle_state.h"

/* PROTOCOL */
/* next_turn    ->  get_skill_context -> execute_action -> ...*/
/* turn_context ->  skill_use_context -> action_report  -> ...*/

battle_turn_context_t battle_system_next_turn(battle_state_t *state, 
                                         const skill_t all_skills[],
                                  const unit_template_t templates[]);

battle_skill_use_context_t 
        battle_system_get_skill_context(battle_state_t *state, 
                                  const battle_unit_t *active, 
                                        const skill_t *skill);
#endif
