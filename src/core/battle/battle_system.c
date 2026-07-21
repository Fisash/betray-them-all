#include <stdlib.h>
#include "core/battle/battle_system.h"
#include "core/battle/battle_mechanics.h"

#ifdef DEBUG
#include <stdio.h>
#endif

static int is_unit_able_to_turn(const struct battle_unit *b)
{
    return (b && b->unit && b->unit->is_alive && b->action_points > 0);
}

static struct battle_unit *find_active_unit_from_side(struct battle_unit side[])
{
    int i;
    for(i = 0; i < MAX_UNITS; i++)
        if(is_unit_able_to_turn(&side[i]))
            return &side[i];        
    return NULL;
}

static struct battle_unit *find_active_unit(struct battle_state *state)
{
    struct battle_unit *side, *active;
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

static void fill_unit_available_skills(struct battle_turn_context *c,
                                      const struct skill all_skills[],
                               const struct unit_template templates[])
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

static void battle_system_next_round(struct battle_state *state)
{
    struct battle_unit *side;
    int pass, i;

    state->round_num++;
    for(pass = 0; pass < 2; pass++)
    {
        side = (pass == 0) ? state->player_units : state->enemy_units;
        for(i = 0; i < MAX_UNITS; i++)
            if(battle_unit_is_alive(&side[i]))
                side[i].action_points = unit_get_mobility(side[i].unit, 
                                                     state->all_items);
    }

}

static int is_active_unit_able_to_useful_turn(const struct battle_state *state,
                                           const struct battle_turn_context *c)
{
    const struct battle_unit *active = c->active_unit;
    if(!c || !c->active_unit) return 0;
    uint16_t max_ap = unit_get_mobility(active->unit, state->all_items);
    return (c->skill_count > 1 || active->action_points == max_ap);
}

struct battle_turn_context battle_system_next_turn(struct battle_state *state, 
                                         const struct skill all_skills[],
                                  const struct unit_template templates[])
{
    struct battle_turn_context result = {0};

    struct battle_unit *active = find_active_unit(state);
    if(active == NULL)
    {
        battle_system_next_round(state);
        active = find_active_unit(state);
    }
    result.active_unit = active;
    fill_unit_available_skills(&result, all_skills, templates);

    if(!is_active_unit_able_to_useful_turn(state, &result))
    {
        active->action_points = 0;
        return battle_system_next_turn(state, all_skills, templates);
    }
    return result;
}

static void add_valid_unit_targets(struct battle_skill_use_context *c, 
                                  struct battle_unit units[MAX_UNITS])
{
    int i;
    for(i = 0; i < MAX_UNITS; i++)
    {
        if(battle_unit_is_alive(&units[i]))
            c->available_targets.units[c->target_count++] = &units[i];
    }
}

static void fill_enemies_and_allies(struct battle_state *state, uint8_t side, 
                        struct battle_unit **enemies, struct battle_unit **allies)
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

struct battle_skill_use_context 
        battle_system_get_skill_context(struct battle_state *state, 
                                  const struct battle_unit *active, 
                                         const struct skill *skill)
{
    struct battle_skill_use_context result = {0};
    result.skill = skill;

    struct battle_unit *my_enemies, *my_allies;
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


static void add_event_report(struct battle_skill_execution_report *out, 
                                     struct battle_event_report report)
{
    out->events[out->target_count++] = report;
}

static int is_any_alive_unit(struct battle_unit units[MAX_UNITS])
{
    int i;
    for (i = 0; i < MAX_UNITS; i++)
        if(battle_unit_is_alive(&units[i]))
            return 1;
    return 0;
}

static void update_battle_state(struct battle_state *state)
{
    if(!is_any_alive_unit(state->player_units))
        state->status = BATTLE_STATUS_LOST;
    else if(!is_any_alive_unit(state->enemy_units))
        state->status = BATTLE_STATUS_WON;
    else
        state->status = BATTLE_STATUS_ACTIVE;
}

struct battle_skill_execution_report 
       battle_system_execute_skill(struct battle_state *state,
                                   struct battle_unit *caster,
                                     enum skill_id skill_id,
                                   struct battle_unit *target)
{
    struct battle_skill_execution_report report = {0};
    report.skill = &(state->all_skills[skill_id]);
    report.caster = caster;

    float damage_scale = 1.0f;
    if(battle_mechanics_check_crit(caster, state->all_items))
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
            struct battle_event_report attack_report = 
                    battle_mechanics_execute_attack(damage_scale, 
                               caster, target, state->all_items);
            add_event_report(&report, attack_report);
            break;
        default:
            break;
    }

    struct battle_event_report *event;
    int is_any_target_died, i, exp_reward;
    for(i = 0, is_any_target_died = 0; i < report.target_count; i++)
    {
        event = &report.events[i];
        if(event->is_target_died)
        {
            exp_reward = event->target->unit->exp_reward;
            unit_add_exp(caster->unit, exp_reward);
            is_any_target_died = 1;

        }
    }

    if(is_any_target_died || report.is_actor_died)
        update_battle_state(state);

    return report;
}

