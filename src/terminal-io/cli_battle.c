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


static void draw_turn_notification(battle_turn_context_t *context)
{
    const char *name = context->active_unit->unit->name;
    printf("It`s %s`s turn!\n", name);
}

static void draw_turn_skill_list(battle_turn_context_t *context)
{
    const skill_t *skill;
    for (int i = 0; i < context->skill_count; i++)
    {
        skill = context->available_skills[i];
        printf("%d. %s\n", (i+1), skill->name);
    }
}

static const skill_t *cli_select_skill(battle_turn_context_t *context)
{
    int selected_index;

    draw_turn_skill_list(context);
    selected_index = cli_choose_digit(1, context->skill_count)-1;

    return context->available_skills[selected_index];
}

void cli_battle_run(battle_state_t *battle, const game_info_t *info)
{
    battle_turn_context_t turn_context;
    const skill_t *skill;

    while(battle->status == BATTLE_STATUS_ACTIVE)
    {
        draw_state(battle);
        turn_context = battle_system_next_turn(battle, info->skills, 
                                              info->unit_templates);
        draw_turn_notification(&turn_context);

        if(turn_context.active_unit->side == IS_PLAYER)
            skill = cli_select_skill(&turn_context);
        else
            skill = battle_bot_select_skill(&turn_context, 
                         SELECTING_RANDOM_NOT_SKIP_SKILL);

        printf("%s was selected %s\n", 
                      turn_context.active_unit->unit->name, 
                      skill->name);
    }
}
