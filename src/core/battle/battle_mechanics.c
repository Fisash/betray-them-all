#include <stdlib.h>

#include "core/battle/battle_mechanics.h"

float battle_mechanics_calc_evasion(battle_unit_t *attacker, 
                                      battle_unit_t *target,
                                  const item_info_t items[])
{
    uint16_t attacker_mobility, target_mobility;
    float evasion, ratio;

    attacker_mobility = unit_get_mobility(attacker->unit, items);
    target_mobility = unit_get_mobility(target->unit, items);

    if(attacker_mobility <= 0)
        return MAX_EVASION;
    ratio = (float)attacker_mobility/ 
            (float)target_mobility; 
    evasion = (ratio*ratio) * PARITY_EVASION;

    if(evasion > MAX_EVASION)
        evasion = MAX_EVASION;
    return evasion;
}

int battle_mechanics_check_evasion(battle_unit_t *attacker,
                                     battle_unit_t *target,
                                 const item_info_t items[])
{
    float evasion_chance = battle_mechanics_calc_evasion(attacker, 
                                                   target, items);
    float random_value = (float)rand() / (float)RAND_MAX;
    return (random_value < evasion_chance);
}

int battle_mechanics_check_crit(battle_unit_t *attacker, 
                              const item_info_t items[])
{
    float crit_chance = (float)(unit_get_crit(attacker->unit, items)/100);
    if (crit_chance > 0.95f)
        crit_chance = 0.95f;
    float random_value = (float)rand() / (float)RAND_MAX;
    return (random_value < crit_chance);
}

void battle_mechanics_apply_damage(battle_unit_t *target, 
                                     uint16_t raw_damage,
                           battle_event_report_t *report, 
                               const item_info_t items[])
{
    uint16_t target_protection = unit_get_protection(target->unit, items);

    uint16_t taken_damage = (raw_damage > target_protection) ?
                            (raw_damage - target_protection) : 0;
    report->hp_change = taken_damage;
    if(target->unit->hp > taken_damage)
        target->unit->hp -= taken_damage;
    else
    {
        target->unit->is_alive = 0;
        report->is_target_died = 1;
    }
}

battle_event_report_t battle_mechanics_execute_attack(float damage_scale,
                                                 battle_unit_t *attacker,
                                                   battle_unit_t *target,
                                               const item_info_t items[])
{
    battle_event_report_t report = {0};
    report.target = target;
    if(battle_mechanics_check_evasion(attacker, target, items))
    {
        report.type = BATTLE_EVENT_TARGET_EVASION;
        return report;
    }
     
    report.type = BATTLE_EVENT_TAKING_DAMAGE;

    uint16_t raw_damage = damage_scale * 
                          unit_get_damage(attacker->unit, items);
    battle_mechanics_apply_damage(target, raw_damage, &report, items);
    return report;
}
