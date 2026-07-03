#include "core/squad_movement.h"

#include "core/state/world.h"

void movement_change_squad_pos_by_order(squad_t *squad)
{
    switch(squad->move_order)
    {
        case MOVE_RIGHT:
            (squad->pos_x)++;
            break;
        case MOVE_LEFT:
            (squad->pos_x)--;
            break;
        case MOVE_UP:
            (squad->pos_y)--;
            break;
        case MOVE_DOWN:
            (squad->pos_y)++;
            break;
        default:
            break;
    }
}

void movement_try_move_squad_by_order(squad_t *squad)
{
    int is_can_move = 0;
    switch(squad->move_order)
    {
        case MOVE_RIGHT:
            is_can_move = squad->pos_x < WORLD_WIDTH - 1;
            break;
        case MOVE_LEFT:
            is_can_move = squad->pos_x > 0;
            break;
        case MOVE_UP:
            is_can_move = squad->pos_y > 0;
            break;
        case MOVE_DOWN:
            is_can_move = squad->pos_y < WORLD_HEIGHT - 1;
            break;
        default:
            break;
    }
    
    if(is_can_move)
        movement_change_squad_pos_by_order(squad);
}

void movement_try_move_squad(squad_t *squad, move_order_t order)
{
    squad->move_order = order;
    movement_try_move_squad_by_order(squad);
}

