#ifndef BATTLE_MECHANICS_H
#define BATTLE_MECHANICS_H

#include "core/state/battle_state.h"

#define MAX_CRIT 0.95f

#define MAX_EVASION 0.95f
#define PARITY_EVASION 0.125f
#define EVASION_C (1.0f/(1.0f-PARITY_EVASION)-1.0f)

int battle_mechanics_check_evasion(battle_unit_t *attacker,
                                     battle_unit_t *target,
                                const item_info_t items[]);

int battle_mechanics_check_crit(battle_unit_t *attacker, 
                             const item_info_t items[]);


void battle_mechanics_apply_damage(battle_unit_t *target, 
                                     uint16_t raw_damage,
                           battle_event_report_t *report, 
                              const item_info_t items[]);

battle_event_report_t battle_mechanics_execute_attack(float damage_scale,
                                                 battle_unit_t *attacker,
                                                   battle_unit_t *target,
                                              const item_info_t items[]);
#endif
