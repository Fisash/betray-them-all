#include <stdio.h>
#include <stdlib.h>

#include "tui/battle.h"
#include "tui/base.h"

#include "core/battle/battle_system.h"
#include "core/battle/battle_bot.h"

#define FIGHT_STATE_SPLITER "-----------------"

static void draw_units(struct battle_unit b_units[])
{
   struct unit *unit;
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

static void draw_state(struct battle_state *battle)
{
    puts(FIGHT_STATE_SPLITER);
    puts("Your units:");
    draw_units(battle->player_units);
    putc('\n', stdout);
    puts("Enemy units:");
    draw_units(battle->enemy_units);
    puts(FIGHT_STATE_SPLITER);
}

static void draw_turn_skill_list(struct battle_turn_context *c)
{
    const struct skill *skill;
    int i;

    printf("Available skills for %s`s turn:\n", 
                   c->active_unit->unit->name);
    for (i = 0; i < c->skill_count; i++)
    {
        skill = c->available_skills[i];
        printf("%d. %s\n", (i+1), skill->name);
    }
}

static void draw_skill_use_target_list(struct battle_skill_use_context *c)
{
    int i;
    const struct battle_unit *battle_unit;

    printf("Available targets for use %s:\n", c->skill->name);
    for (i = 0; i < c->target_count; i++)
    {
        battle_unit = c->available_targets.units[i];
        printf("%d. %s\n", (i+1), battle_unit->unit->name);
    }
}

static const struct skill *cli_select_skill(struct battle_turn_context *c)
{
    int selected_index;
    draw_turn_skill_list(c);
    selected_index = cli_base_choose_number(1, c->skill_count)-1;
    return c->available_skills[selected_index];
}

static struct battle_unit *cli_select_target(struct battle_skill_use_context *c)
{
    int selected_index;
    draw_skill_use_target_list(c);
    selected_index = cli_base_choose_number(1, c->target_count)-1;
    return c->available_targets.units[selected_index];
}

static void notification_turn_started(struct battle_turn_context *c, int round)
{
    printf("Round %d. It`s %s`s turn.\n", 
      round, c->active_unit->unit->name);
}

static void notification_event(struct battle_event_report *r)
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

static void notification_turn_result(struct battle_skill_execution_report *r)
{
    int i;
    printf("%s casted %s.\n", r->caster->unit->name, r->skill->name);
    if(r->is_crit)
        puts("It was critiсal cast\n"); 
    for (i = 0; i < r->target_count; i++)
        notification_event(&r->events[i]); 
}

void cli_battle_run(struct battle_state *battle, const struct game_info *info)
{
    struct battle_turn_context turn_context;
    struct battle_skill_use_context skill_use_context;
    struct battle_skill_execution_report report;

    struct battle_unit *target = NULL;
    int is_player_turn;
    const struct skill *skill = NULL;
    enum skill_id skill_id;

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
