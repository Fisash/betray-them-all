#include <stdio.h>

#include "terminal-io/cli_battle.h"

#include "core/battle_system.h"

static void draw_units(battle_unit_t b_units[])
{
   int count = 0;
   unit_t *unit;
   for (int i = 0; i < MAX_UNITS; i++)
   {
        if(!battle_unit_is_alive(&b_units[i]))
            continue;

        unit = b_units[i].unit;
        printf("%d. %s(%d/%d)\n", ++count, unit->name, unit->hp, unit->max_hp);
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

static void draw_turn_context(battle_turn_context_t *context)
{
    const skill_t *skill;
    int skill_count = 0;

    const char *name = context->active_unit->unit->name;
    printf("It`s %s`s turn!\n", name);

    for (int i = 0; i < MAX_TURN_SKILL_COUNT; i++)
    {
        skill = context->available_skills[i];
        if(skill)
            printf("%d. %s\n", ++skill_count, skill->name);
    }
}

void cli_battle_run(battle_state_t *battle, const game_info_t *info)
{
    battle_turn_context_t turn_context;

    while(battle->status == BATTLE_STATUS_ACTIVE)
    {
        draw_state(battle);
        turn_context = battle_system_next_turn(battle, info->skills, 
                                              info->unit_templates);
        draw_turn_context(&turn_context);
        for(;;)
        {

        }
    }
}
