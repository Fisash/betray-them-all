#include <stdlib.h>

#include "core/battle/battle_system.h"

#ifdef DEBUG
#include <stdio.h>
#endif

static int is_unit_able_to_turn(const battle_unit_t *b)
{
    return (b && b->unit && b->unit->is_alive && b->action_points > 0);
}

static battle_unit_t *find_active_unit_from_side(battle_unit_t side[])
{
    for(int i = 0; i < MAX_UNITS; i++)
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
    int is_include_bit, is_enough_ap, turn_skill_count;

    unit_fill_available_skills(available, c->active_unit->unit, 
                                        all_skills, templates);
    turn_skill_count = 0;
    for (int i = 0; i < SKILL_COUNT; i++)
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

static void fill_unit_targets(battle_skill_use_context_t *context, 
                                   battle_unit_t units[MAX_UNITS],
                                                  uint8_t offset)
{
    for(int i = 0; i < MAX_UNITS; i++)
        context->available_targets.units[offset+i] = &units[i];
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

    if(is_skill_target_class_require_choice(skill->target_class))
    {
        if(skill->target_class == SKILL_TARGET_ENEMY_UNIT)
            fill_unit_targets(&result, my_enemies, 0);
        if(skill->target_class == SKILL_TARGET_ALLY_UNIT)
            fill_unit_targets(&result, my_allies, 0);
        if(skill->target_class == SKILL_TARGET_ANY_UNIT)
        {
            fill_unit_targets(&result, my_enemies, 0);
            fill_unit_targets(&result, my_allies, MAX_UNITS);
        }
    }

    return result;
}

/*
void battle_system_select_target(battle_state_t *state, skill_t *skill)
{

}*/
