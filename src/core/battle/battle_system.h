#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "core/state/battle_state.h"

/* PROTOCOL */
/* next_turn    ->  get_skill_context -> execute_action -> ...*/
/* turn_context ->  skill_use_context -> action_report  -> ...*/

struct battle_turn_context battle_system_next_turn(struct battle_state *state, 
                                         const struct skill all_skills[],
                                 const struct unit_template templates[]);

struct battle_skill_use_context 
        battle_system_get_skill_context(struct battle_state *state, 
                                  const struct battle_unit *active, 
                                        const struct skill *skill);
struct battle_skill_execution_report 
       battle_system_execute_skill(struct battle_state *state,
                                   struct battle_unit *caster,
                                     enum skill_id skill_id,
                                  struct battle_unit *target);

#endif
