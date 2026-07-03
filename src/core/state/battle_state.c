#include <string.h>

#include "core/state/battle_state.h"

static void battle_unit_init(battle_unit_t *b, unit_t *unit, 
                       uint8_t side, uint16_t action_points)
{
    b->unit = unit;
    b->action_points = action_points;
    b->side = side;
}

int battle_unit_is_alive(battle_unit_t *b)
{
    return (b && b->unit && b->unit->is_alive);
}

void battle_state_init(battle_state_t *b, squad_t *squad)
{
    b->player_unit_count = 0;
    b->enemy_unit_count = 0;

    unit_t *unit;
    battle_unit_t *battle_unit;
    for(int i = 0; i < MAX_UNITS; i++)
    {
        unit = &squad->units[i]; 
        if(unit->is_alive)
        {
            battle_unit = &(b->player_units[b->player_unit_count++]);
            battle_unit_init(battle_unit, unit, IS_PLAYER, 
                                     START_ACTION_POINTS);
        }
    }

    memset(b->enemy_storage, 0, sizeof(b->enemy_storage));
    b->status = BATTLE_STATUS_ACTIVE;
}

int battle_state_add_enemy(battle_state_t *b, const unit_t *enemy)
{
    battle_unit_t *battle_unit;
    unit_t *my_enemy;
    for(int i = 0; i < MAX_UNITS; i++)
    {
        battle_unit = &b->enemy_units[i];
        my_enemy = &b->enemy_storage[i];
        if(!battle_unit_is_alive(battle_unit))
        {
            *my_enemy  = *enemy;
            battle_unit_init(battle_unit, my_enemy, IS_ENEMY, 
                                        START_ACTION_POINTS);
            b->enemy_unit_count++;
            return 0;
        }
    }

    return 1;
}
