#include <stdio.h>

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

static void draw_turn_skill_list(battle_turn_context_t *c)
{
    printf("Available skills for %s`s turn:\n", 
                   c->active_unit->unit->name);

    const skill_t *skill;
    for (int i = 0; i < c->skill_count; i++)
    {
        skill = c->available_skills[i];
        printf("%d. %s\n", (i+1), skill->name);
    }
}

static void draw_skill_use_target_list(battle_skill_use_context_t *c)
{
    printf("Available targets for use %s:\n", c->skill->name);

    const battle_unit_t *battle_unit;
    for (int i = 0; i < c->target_count; i++)
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

void cli_battle_run(battle_state_t *battle, const game_info_t *info)
{
    battle_turn_context_t turn_context;
    battle_skill_use_context_t skill_use_context;

    battle_unit_t *target;
    const skill_t *skill;

    while(battle->status == BATTLE_STATUS_ACTIVE)
    {
        draw_state(battle);
        turn_context = battle_system_next_turn(battle, info->skills, 
                                              info->unit_templates);
        printf("It`s %s`s turn.\n", 
                      turn_context.active_unit->unit->name);

        if(turn_context.active_unit->side == IS_PLAYER)
            skill = cli_select_skill(&turn_context);
        else
            skill = battle_bot_select_skill(&turn_context, 
                         SELECTING_RANDOM_NOT_SKIP_SKILL);

        if(skill_is_target_class_require_choice(skill->target_class))
        {
            skill_use_context = battle_system_get_skill_context(battle, 
                                      turn_context.active_unit, skill);

            if(turn_context.active_unit->side == IS_PLAYER)
                target = cli_select_target(&skill_use_context);
            else
                target = battle_bot_select_target(&skill_use_context,
                                           SELECTING_LESS_HP_TARGET);
        }

        printf("%s selected %s", 
                      turn_context.active_unit->unit->name, 
                      skill->name);
        if(battle_unit_is_alive(target))
            printf(" for target %s", target->unit->name);
        putc('\n', stdout);
    }
}
