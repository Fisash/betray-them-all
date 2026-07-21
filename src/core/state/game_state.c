#include <stdlib.h>
#include <string.h>

#include "core/state/game_state.h"

static void give_start_items(struct squad *squad, const struct game_info *info)
{
    struct item test_item, test_item2, test_weapon;
    item_init(&test_item, info->items, ITEM_RUBY);
    item_init(&test_item2, info->items, ITEM_MALACHITE);

    item_init(&test_weapon, info->items, WEAPON_HUMMER);

    squad_add_item(squad, &test_item);
    squad_add_item(squad, &test_item2);
    squad_add_item(squad, &test_weapon);
}

static void give_start_units(struct squad *squad, const struct game_info *info)
{
    struct unit start_unit1, start_unit2;
    unit_init(&start_unit1, "fucker", WARRIOR, 
        info->unit_templates, info->items);
    unit_init(&start_unit2, "absabd", ROGUE, 
        info->unit_templates, info->items);

    squad_add_unit(squad, &start_unit1);
    squad_add_unit(squad, &start_unit2);
}

void game_state_init(struct game_state *game_state, const struct game_info *info)
{
    memset(game_state, 0, sizeof(struct game_state));

    game_state->days = 1;
    world_generate(&(game_state->world), info->items);

    struct squad* squad = &(game_state->squad);
    squad_init(squad, 1, 5, info->items);
    give_start_units(squad, info);
    give_start_items(squad, info);
    game_state->active_event_id = EVENT_NONE;
}
