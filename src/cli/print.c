#include <stdio.h>
#include "print.h"

#define CELL_VIEW_MEADOW '-'
#define CELL_VIEW_FOREST '!'
#define CELL_VIEW_MOUNTAIN '^'
#define CELL_VIEW_VILLAGE 'v'

#define CELL_VIEW_SQUAD 'p'

#define CELL_VIEW_DEFAULT '#'

void print_base_info(const struct game_state *game, 
                     const struct game_info *info)
{
    const struct cell *squad_cell;

    const struct squad *squad = &game->squad;
    const struct world *world = &game->world;

    printf("Gold: %d; Provision: %d; Day: %d\n",
        squad->gold, squad->provision, game->days);

    squad_cell = &world->cells[squad->pos_y][squad->pos_x];
    printf("Location: %s (%d, %d)\n", 
        info->cells_info[squad_cell->type_id].title,
        squad->pos_x, squad->pos_y);
}

void print_unit_list(const struct squad *squad)
{
    uint16_t i, unit_count;
    const struct unit *unit;

    for(i = 0; i < SQUAD_MAX_UNITS; i++)
    {
        unit = &squad->units[i];
        if(!unit->is_alive)
            continue;
        unit_count++;
        printf("%d. %s (%d/%d)\n", unit_count, unit->name, 
                                 unit->hp, unit->max_hp);
    } 
}

void print_item_list(const struct squad *squad)
{
    uint16_t i, item_count;
    const struct item *item;

    for(i = 0, item_count = 0; i < SQUAD_MAX_ITEMS; i++)
    {
        item = &squad->inventory[i];
        if(!item || item->id == ITEM_NONE)
            continue;
        item_count++;
        printf("%d. %s \n", item_count, 
            squad->items_info[item->id].title);
    }
}

static char get_cell_view(enum cell_type_id id)
{
    switch (id)
    {
        case CELL_TYPE_MEADOW:
            return CELL_VIEW_MEADOW;
            break;
        case CELL_TYPE_FOREST:
            return CELL_VIEW_FOREST;
            break;
        case CELL_TYPE_MOUNTAIN:
            return CELL_VIEW_MOUNTAIN;
            break;
        case CELL_TYPE_VILLAGE:
            return CELL_VIEW_VILLAGE;
            break;
        default:
            return CELL_VIEW_DEFAULT;
            break;
    }
}

void print_world(struct world *world, struct squad *squad)
{
    enum cell_type_id id;
    char c;
    int x, y, is_squad_cell;

    for (y = 0; y < WORLD_HEIGHT; y++)
    {
        for (x = 0; x < WORLD_WIDTH; x++)
        {
            id = world->cells[y][x].type_id;
            is_squad_cell = (x == squad->pos_x && y == squad->pos_y);
            c = is_squad_cell ? CELL_VIEW_SQUAD : get_cell_view(id);
            putc(c, stdout);
        }
        putc('\n', stdout);
    }
}

void print_unit_info(struct unit *unit, const struct item_info items[],
                     const struct unit_template templates[])
{
    printf("Name: %s, (%s)\n", unit->name, 
           templates[unit->template_id].name);
    printf("LVL: %d. EXP: %d/%d\n", unit->level, unit->exp, 
                                  unit->exp_for_next_level);
    printf("HP: %d/%d\n", unit->hp, unit->max_hp);
    printf("STR: %d\nAGL: %d\nWIL: %d\nINT: %d\n", 
           unit->stats.strength, unit->stats.agility, 
           unit->stats.will,unit->stats.intelligence);

    printf("Weapon: ");
    print_item_info(&unit->weapon, items, unit);
    printf("Armor: ");
    print_item_info(&unit->armor, items, unit);
    if(unit->unspent_stat_points > 0)
        printf("Unspent stat leveling points: %d", 
                       unit->unspent_stat_points);
}

void print_stat_with_scaling(const char *label, int base_value, int real_value,
                             const struct scaling_group *scalings,
                             int is_show_real)
{
    if(is_show_real)
        printf("%s: %d(%d) (%c%c%c%c)", label, base_value, real_value, 
                                 scale_get_rank_view(scalings->strength),
                                  scale_get_rank_view(scalings->agility),
                                     scale_get_rank_view(scalings->will),
                            scale_get_rank_view(scalings->intelligence));
    else
        printf("%s: %d (%c%c%c%c)", label, base_value, 
                 scale_get_rank_view(scalings->strength),
                  scale_get_rank_view(scalings->agility),
                     scale_get_rank_view(scalings->will),
            scale_get_rank_view(scalings->intelligence));
}
                                    
void print_item_info(const struct item *item, const struct item_info info[],
                                                          struct unit *unit)
{
    const struct item_info *item_info = &info[item->id];
    printf("%s (cost: %d) - ", item_info->title, item->cost);
    switch(item_info->type)
    {
        case ITEM_TYPE_WEAPON:
            int base_damage = item->props.weapon.damage;
            int real_damage = unit ? unit_get_damage(unit, info) : 0;
            print_stat_with_scaling("Damage", base_damage, real_damage,
                              &item_info->props.weapon.damage.scalings, 
                                                         unit != NULL);
            putc(' ', stdout);
            int base_crit = item->props.weapon.crit;
            int real_crit = unit ? unit_get_crit(unit, info) : 0;
            print_stat_with_scaling("Crit", base_crit, real_crit,
                          &item_info->props.weapon.crit.scalings, 
                                                   unit != NULL);
            break; 
        case ITEM_TYPE_ARMOR:
            int base_protection = item->props.armor.protection;
            int real_protection = unit ? unit_get_protection(unit, info) : 0;
            print_stat_with_scaling("Protection", base_protection, 
                                                  real_protection,
                      &item_info->props.armor.protection.scalings, 
                                                    unit != NULL);

            putc(' ', stdout);
            int base_mobility = item->props.armor.mobility;
            int real_mobility = unit ? unit_get_mobility(unit, info) : 0;
            print_stat_with_scaling("Mobility", base_mobility, 
                                                real_mobility,
                    &item_info->props.armor.mobility.scalings, 
                                                unit != NULL);
            break; 
        default:
            break;
    }
    printf("\n");
}
