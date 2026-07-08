#include <stdio.h>
#include <stdlib.h>

#include "terminal-io/cli_battle.h"

#include "core/battle/battle_system.h"
#include "core/battle/battle_bot.h"

int cli_choose_digit(uint8_t min, uint8_t max)
{
    int input;
    for(;;) 
    {
        input = getc(stdin);
        if (input >= '0'+min && input <= '0'+max) 
            return input-'0';
    }
}

static void draw_units(battle_unit_t b_units[])
{
   unit_t *unit;
   int count, i;

   count = 0;
   for (i = 0; i < MAX_UNITS; i++)
   {
        if(!battle_unit_is_alive(&b_units[i]))
            continue;

        unit = b_units[i].unit;
        printf("%d. %s(%d/%d)\n", ++count, unit->name, 
                                  unit->hp, unit->max_hp);
   }
}

static void draw_state(battle_state_t *battle)
{
    puts("Your units:\n");
    draw_units(battle->player_units);
    puts("-----------------");
    puts("Enemy units:\n");
    draw_units(battle->enemy_units);
}

static void draw_turn_skill_list(battle_turn_context_t *c)
{
    const skill_t *skill;
    int i;

    printf("Available skills for %s`s turn:\n", 
                   c->active_unit->unit->name);
    for (i = 0; i < c->skill_count; i++)
    {
        skill = c->available_skills[i];
        printf("%d. %s\n", (i+1), skill->name);
    }
}

static void draw_skill_use_target_list(battle_skill_use_context_t *c)
{
    int i;
    const battle_unit_t *battle_unit;

    printf("Available targets for use %s:\n", c->skill->name);
    for (i = 0; i < c->target_count; i++)
    {
        battle_unit = c->available_targets.units[i];
        printf("%d. %s\n", (i+1), battle_unit->unit->name);
    }
}

static const skill_t *cli_select_skill(battle_turn_context_t *c)
{
    int selected_index;
    draw_turn_skill_list(c);
    selected_index = cli_choose_digit(1, c->skill_count)-1;
    return c->available_skills[selected_index];
}

static battle_unit_t *cli_select_target(battle_skill_use_context_t *c)
{
    int selected_index;
    draw_skill_use_target_list(c);
    selected_index = cli_choose_digit(1, c->target_count)-1;
    return c->available_targets.units[selected_index];
}

static void notification_turn_started(battle_turn_context_t *c, int round)
{
    printf("Round %d. It`s %s`s turn.\n", 
      round, c->active_unit->unit->name);
}

static void notification_event(battle_event_report_t *r)
{

    printf(r->target->unit->name);
    switch(r->type)
    {
        case BATTLE_EVENT_TAKING_DAMAGE:
            printf(" taked damage for %d hp", r->hp_change);
            break;
        case BATTLE_EVENT_HEALING:
            printf(" taked heal for %d hp", r->hp_change);
            break;
        case BATTLE_EVENT_TARGET_EVASION:
            printf(" dodged the attack");
    }

    if(r->is_target_died)
            printf(" and died.");
    putc('\n', stdout);
}

static void notification_turn_result(battle_skill_execution_report_t *r)
{
    int i;
    printf("%s casted %s.\n", r->caster->unit->name, r->skill->name);
    if(r->is_crit)
        puts("It was critiсal cast\n"); 
    for (i = 0; i < r->target_count; i++)
        notification_event(&r->events[i]); 
}

void cli_battle_run(battle_state_t *battle, const game_info_t *info)
{
    battle_turn_context_t turn_context;
    battle_skill_use_context_t skill_use_context;
    battle_skill_execution_report_t report;

    battle_unit_t *target = NULL;
    int is_player_turn;
    const skill_t *skill = NULL;
    skill_id_t skill_id;

    while(battle->status == BATTLE_STATUS_ACTIVE)
    {
        draw_state(battle);
        turn_context = battle_system_next_turn(battle, info->skills, 
                                              info->unit_templates);
        is_player_turn = turn_context.active_unit->side == IS_PLAYER;

        notification_turn_started(&turn_context, battle->round_num);

        skill = is_player_turn                       ?
                cli_select_skill(&turn_context)      : 
                battle_bot_select_skill(&turn_context, 
                     SELECTING_RANDOM_NOT_SKIP_SKILL);

        if(skill_is_target_class_require_choice(skill->target_class))
        {
            skill_use_context = battle_system_get_skill_context(battle, 
                                      turn_context.active_unit, skill);
            target = is_player_turn                             ?
                     cli_select_target(&skill_use_context)      :
                     battle_bot_select_target(&skill_use_context,
                                       SELECTING_LESS_HP_TARGET);
        }

        skill_id = skill_get_id(skill, battle->all_skills);
        report = battle_system_execute_skill(battle, 
                           turn_context.active_unit, 
                                  skill_id, target);
        notification_turn_result(&report);
    }
    
    if(battle->status == BATTLE_STATUS_LOST)
    {
        puts("You lost!\n");
        exit(0);
    }
}
