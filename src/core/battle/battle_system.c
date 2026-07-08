#include <stdlib.h>

#include "core/battle/battle_system.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define MAX_EVASION 0.95f
#define PARITY_EVASION 0.1f

static int is_unit_able_to_turn(const battle_unit_t *b)
{
    return (b && b->unit && b->unit->is_alive && b->action_points > 0);
}

static battle_unit_t *find_active_unit_from_side(battle_unit_t side[])
{
    int i;
    for(i = 0; i < MAX_UNITS; i++)
        if(is_unit_able_to_turn(&side[i]))
            return &side[i];        
    return NULL;
}

static battle_unit_t *find_active_unit(battle_state_t *state)
{
    battle_unit_t *side, *active;
    int is_player_turn, r, pass;

    r = rand() % 2;
    for(pass = 0; pass < 2; pass++)
    {
        is_player_turn = (pass == r);
        side = is_player_turn ? state->player_units : state->enemy_units;
        active = find_active_unit_from_side(side);
        if(active)
            return active;
    }

    return NULL;
}

static void fill_unit_available_skills(battle_turn_context_t *c,
                                      const skill_t all_skills[],
                               const unit_template_t templates[])
{
    skills_mask_t available;
    int is_include_bit, is_enough_ap, turn_skill_count, i;

    unit_fill_available_skills(available, c->active_unit->unit, 
                                        all_skills, templates);
    turn_skill_count = 0;
    for (i = 0; i < SKILL_COUNT; i++)
    {
        is_include_bit = skills_mask_is_include_bit(available, i);
        is_enough_ap = (c->active_unit->action_points >= 
                       all_skills[i].action_points_cost);

        if(is_include_bit && is_enough_ap)
        {
            c->available_skills[turn_skill_count++] = &all_skills[i];
#ifdef DEBUG
            printf("[DEBUG]skill: %s\n", all_skills[i].name);
#endif
        }
    }
    c->skill_count = turn_skill_count;
}

static void battle_system_next_round(battle_state_t *state)
{
    battle_unit_t *side;
    int pass, i;

    state->round_num++;
    for(pass = 0; pass < 2; pass++)
    {
        side = (pass == 0) ? state->player_units : state->enemy_units;
        for(i = 0; i < MAX_UNITS; i++)
            side[i].action_points = START_ACTION_POINTS;
    }

}

battle_turn_context_t battle_system_next_turn(battle_state_t *state, 
                                         const skill_t all_skills[],
                                  const unit_template_t templates[])
{
    battle_turn_context_t result = {0};

    battle_unit_t *active = find_active_unit(state);
    if(active == NULL)
    {
        battle_system_next_round(state);
        active = find_active_unit(state);
    }
    result.active_unit = active;
    fill_unit_available_skills(&result, all_skills, templates);
    return result;
}

static void add_valid_unit_targets(battle_skill_use_context_t *c, 
                                  battle_unit_t units[MAX_UNITS])
{
    int i;
    for(i = 0; i < MAX_UNITS; i++)
    {
        if(battle_unit_is_alive(&units[i]))
            c->available_targets.units[c->target_count++] = &units[i];
    }
}

static void fill_enemies_and_allies(battle_state_t *state, uint8_t side, 
                        battle_unit_t **enemies, battle_unit_t **allies)
{
    if(side == IS_PLAYER)
    {
        *enemies = state->enemy_units;
        *allies = state->player_units;
    }
    else
    {
        *enemies = state->player_units;
        *allies = state->enemy_units;
    }
}

battle_skill_use_context_t 
        battle_system_get_skill_context(battle_state_t *state, 
                                  const battle_unit_t *active, 
                                         const skill_t *skill)
{
    battle_skill_use_context_t result = {0};
    result.skill = skill;

    battle_unit_t *my_enemies, *my_allies;
    fill_enemies_and_allies(state, active->side, &my_enemies, &my_allies);

    if(skill_is_target_class_require_choice(skill->target_class))
    {
        switch(skill->target_class)
        {
            case SKILL_TARGET_ENEMY_UNIT:
                add_valid_unit_targets(&result, my_enemies);
                break;
            case SKILL_TARGET_ALLY_UNIT:
                add_valid_unit_targets(&result, my_allies);
                break;
            case SKILL_TARGET_ANY_UNIT:
                add_valid_unit_targets(&result, my_enemies);
                add_valid_unit_targets(&result, my_allies);
                break;
            default:
                break;
        }
    }

    return result;
}

static float calc_target_evasion(battle_unit_t *attacker, 
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

static int check_target_evasion(battle_unit_t *attacker,
                                  battle_unit_t *target,
                               const item_info_t items[])
{
    float evasion_chance = calc_target_evasion(attacker, target, items);
    float random_value = (float)rand() / (float)RAND_MAX;
    return (random_value < evasion_chance);
}

static int check_crit(battle_unit_t *attacker, 
                     const item_info_t items[])
{
    float crit_chance = (float)(unit_get_crit(attacker->unit, items)/100);
    if (crit_chance > 0.95f)
        crit_chance = 0.95f;
    float random_value = (float)rand() / (float)RAND_MAX;
    return (random_value < crit_chance);
}

static void apply_damage(battle_unit_t *target, uint16_t raw_damage,
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

static battle_event_report_t execute_attack(float damage_scale,
                                       battle_unit_t *attacker,
                                         battle_unit_t *target,
                                     const item_info_t items[])
{
    battle_event_report_t report = {0};
    report.target = target;
    if(check_target_evasion(attacker, target, items))
    {
        report.type = BATTLE_EVENT_TARGET_EVASION;
        return report;
    }
     
    report.type = BATTLE_EVENT_TAKING_DAMAGE;

    uint16_t raw_damage = damage_scale * 
                          unit_get_damage(attacker->unit, items);
    apply_damage(target, raw_damage, &report, items);
    return report;
}

static void add_event_report(battle_skill_execution_report_t *out, 
                                     battle_event_report_t report)
{
    out->events[out->target_count++] = report;
}

static int is_any_alive_unit(battle_unit_t units[MAX_UNITS])
{
    int i;
    for (i = 0; i < MAX_UNITS; i++)
        if(battle_unit_is_alive(&units[i]))
            return 1;
    return 0;
}

static void update_battle_state(battle_state_t *state)
{
    if(!is_any_alive_unit(state->player_units))
    {
        state->status = BATTLE_STATUS_LOST;
        return;
    }
   
    if(!is_any_alive_unit(state->enemy_units))
        state->status = BATTLE_STATUS_WON;
    else
        state->status = BATTLE_STATUS_ACTIVE;
}

static int is_any_unit_died_after_execute(battle_skill_execution_report_t *r)
{
    int i;
    for(i = 0; i < r->target_count; i++)
        if(r->events[i].is_target_died)
            return 1;
    return 0;
}

battle_skill_execution_report_t 
       battle_system_execute_skill(battle_state_t *state,
                                   battle_unit_t *caster,
                                     skill_id_t skill_id,
                                   battle_unit_t *target)
{
    battle_skill_execution_report_t report = {0};
    report.skill = &(state->all_skills[skill_id]);
    report.caster = caster;

    float damage_scale = 1.0f;
    if(check_crit(caster, state->all_items))
    {
        damage_scale = 2.0f;
        report.is_crit = 1;
    }
    
    caster->action_points -= report.skill->action_points_cost;
    switch (skill_id)
    {
        case SKILL_SKIP_TURN:
            caster->action_points = 0;
              break;
        case SKILL_USUAL_SLASH:
        case SKILL_USUAL_STABBING:
        case SKILL_MAUL:
            battle_event_report_t attack_report = execute_attack(damage_scale, 
                                            caster, target, state->all_items);
            add_event_report(&report, attack_report);
            break;
        default:
            break;
    }

    if(is_any_unit_died_after_execute(&report))
        update_battle_state(state);

    return report;
}

