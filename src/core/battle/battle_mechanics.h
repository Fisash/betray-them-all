#ifndef BATTLE_MECHANICS_H
#define BATTLE_MECHANICS_H

#include "core/state/battle_state.h"

#define MAX_CRIT 0.95f

#define MAX_EVASION 0.95f
#define PARITY_EVASION 0.125f
#define EVASION_C (1.0f/(1.0f-PARITY_EVASION)-1.0f)

int battle_mechanics_check_evasion(struct battle_unit *attacker,
                                     struct battle_unit *target,
                                const struct item_info items[]);

int battle_mechanics_check_crit(struct battle_unit *attacker, 
                             const struct item_info items[]);


void battle_mechanics_apply_damage(struct battle_unit *target, 
                                     uint16_t raw_damage,
                           struct battle_event_report *report, 
                              const struct item_info items[]);

struct battle_event_report battle_mechanics_execute_attack(float damage_scale,
                                                 struct battle_unit *attacker,
                                                   struct battle_unit *target,
                                              const struct item_info items[]);
#endif
