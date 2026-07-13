#include <string.h>

#include "core/defs/items_info.h"

char scale_get_rank_view(scale_rank_t rank)
{
    switch(rank)
    {
        case SCALE_N: return '-';
        case SCALE_E: return 'E';
        case SCALE_D: return 'D';
        case SCALE_C: return 'C';
        case SCALE_B: return 'B';
        case SCALE_A: return 'A';
        case SCALE_S: return 'S';
        default:      return '-';
    }
}

static float get_scale_multiplier(scale_rank_t rank)
{
    switch(rank)
    {
        case SCALE_N: return 0.0f;
        case SCALE_E: return 1.0f;
        case SCALE_D: return 1.5f;
        case SCALE_C: return 2.0f;
        case SCALE_B: return 2.8f;
        case SCALE_A: return 3.5f;
        case SCALE_S: return 5.0f;
        default:      return 0.0f;
    }
}

static float get_stat_scale_profit(scale_rank_t rank, uint16_t stat)
{
    return (get_scale_multiplier(rank) * (stat)/(MAX_STAT_VALUE));
}

uint16_t scale_param(uint16_t source, scaling_group_t scalings, 
                                            unit_stats_t stats)
{
    float bonus = 1;
    bonus += get_stat_scale_profit(scalings.strength, 
                                     stats.strength);
    bonus += get_stat_scale_profit(scalings.agility, 
                                     stats.agility);
    bonus += get_stat_scale_profit(scalings.will, 
                                     stats.will);
    bonus += get_stat_scale_profit(scalings.intelligence, 
                                     stats.intelligence);

    return (uint16_t)(source * bonus);
}

static void init_item(item_info_t *item, const char *title, 
                                   const char *description,
                                   uint16_t cost)
{
    item->type = ITEM_TYPE_GENERIC;
    item->cost = cost;
    strcpy(item->title, title);
    strcpy(item->description, description);
}

static void init_armor(item_info_t *armor, const char *title, 
                                     const char *description, 
              param_spec_t protection, param_spec_t mobility)
{
    init_item(armor, title, description, 0);
    armor->type = ITEM_TYPE_ARMOR;

    armor->props.armor.protection = protection;
    armor->props.armor.mobility = mobility;

}

static void init_weapon(item_info_t *weapon, const char *title, 
                                       const char *description, 
                        param_spec_t damage, param_spec_t crit)
{
    init_item(weapon, title, description, 0);
    weapon->type = ITEM_TYPE_WEAPON;

    weapon->props.weapon.damage = damage;
    weapon->props.weapon.crit = crit;
}

static void init_provision(item_info_t *provision, const char *title,
                              const char *description, uint16_t cost, 
                                    uint8_t provision_increase_value)
{
    init_item(provision, title, description, cost);
    provision->type = ITEM_TYPE_PROVISION;
    provision->props.provision_increase_value = provision_increase_value;
}

static void param_spec_init(param_spec_t *param, uint16_t min, uint16_t max,
                                         scale_rank_t str, scale_rank_t agi, 
                                        scale_rank_t wil, scale_rank_t inte)
{
    param->min = min;
    param->max = max;
    param->scalings.strength = str;
    param->scalings.agility = agi;
    param->scalings.will = wil;
    param->scalings.intelligence = inte;
}

void items_info_load(item_info_t items[])
{
    param_spec_t damage, crit, mobility, protection;

    memset(items, 0, sizeof(item_info_t)*ALL_ITEMS_COUNT);

    /* armors */
    param_spec_init(&protection, 0, 0,
                    SCALE_N, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&mobility, 10, 10,
                    SCALE_N, SCALE_A, SCALE_N, SCALE_N);
    init_armor(&items[ARMOR_NUDE],  "Nude",
                       "No armor at all", protection, mobility);

    param_spec_init(&protection, 2, 2,
                    SCALE_N, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&mobility, 10, 11,
                    SCALE_N, SCALE_A, SCALE_N, SCALE_N);
    init_armor(&items[ARMOR_FABRIC],  "Fabric armor",
                       "Light armor from fabric", protection, mobility);

    param_spec_init(&protection, 4, 5,
                    SCALE_N, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&mobility, 7, 8,
                    SCALE_D, SCALE_B, SCALE_N, SCALE_N);
    init_armor(&items[ARMOR_LEATHER], "Leather armor",
                       "Light armor from leather", protection, mobility);

    param_spec_init(&protection, 8, 9,
                    SCALE_N, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&mobility, 3, 3,
                    SCALE_C, SCALE_D, SCALE_N, SCALE_N);
    init_armor(&items[ARMOR_IRON],    "Iron armor",
                          "Medium weight armor", protection, mobility);

    param_spec_init(&protection, 11, 12,
                    SCALE_N, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&mobility, 1, 1,
                    SCALE_B, SCALE_N, SCALE_N, SCALE_N);
    init_armor(&items[ARMOR_SHELL],   "Shell armor",
                              "Best protection", protection, mobility);

    /* weapons */
    param_spec_init(&damage, 2, 2,
                    SCALE_B, SCALE_D, SCALE_N, SCALE_N);
    param_spec_init(&crit, 3, 3,
                    SCALE_E, SCALE_E, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_FISTS], "Fists",
        "Bare hands", damage, crit);

    param_spec_init(&damage, 6, 6,
                    SCALE_A, SCALE_D, SCALE_N, SCALE_N);
    param_spec_init(&crit, 6, 6,
                    SCALE_B, SCALE_E, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_CLAW], "Claw",
        "Sharp animal claws", damage, crit);

    param_spec_init(&damage, 6, 7,
                    SCALE_B, SCALE_D, SCALE_N, SCALE_N);
    param_spec_init(&crit, 5, 5,
                    SCALE_D, SCALE_N, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_SHORT_SWORD], "Short sword",
        "Small but light-weight sword", damage, crit);

    param_spec_init(&damage, 9, 10,
                    SCALE_B, SCALE_D, SCALE_N, SCALE_N);
    param_spec_init(&crit, 5, 5,
                    SCALE_D, SCALE_N, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_SWORD], "Sword",
        "Standart size and damage sword", damage, crit);

    param_spec_init(&damage, 4, 5,
                    SCALE_C, SCALE_B, SCALE_N, SCALE_N);
    param_spec_init(&crit, 10, 10,
                    SCALE_B, SCALE_B, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_DAGGER], "Dagger",
        "Comact but deadly dagger in skilled hands ", damage, crit);

    param_spec_init(&damage, 5, 6,
                    SCALE_B, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&crit, 5, 5,
                    SCALE_D, SCALE_N, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_MACE], "Mace",
        "Fast and light-weight mace", damage, crit);

    param_spec_init(&damage, 11, 12,
                    SCALE_B, SCALE_N, SCALE_N, SCALE_N);
    param_spec_init(&crit, 8, 8,
                    SCALE_C, SCALE_N, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_HUMMER], "Hummer",
        "Huge war hummer", damage, crit);

    param_spec_init(&damage, 8, 9,
                    SCALE_B, SCALE_C, SCALE_N, SCALE_N);
    param_spec_init(&crit, 5, 5,
                    SCALE_D, SCALE_D, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_SPEAR], "Spear",
        "Long and simple spear", damage, crit);

    param_spec_init(&damage, 12, 13,
                    SCALE_A, SCALE_C, SCALE_N, SCALE_N);
    param_spec_init(&crit, 8, 8,
                    SCALE_C, SCALE_D, SCALE_N, SCALE_N);
    init_weapon(&items[WEAPON_HALBERD], "Halberd",
        "Powerful roal halberd", damage, crit);

    /*provision*/
    init_provision(&items[ITEM_PROVISION_BAG], "Provision bag",
                   "Increases provision points by 15", 10, 15);

    /*other items*/
    init_item(&items[ITEM_MALACHITE], "Malachite", "Basic gemstone", 30);
    init_item(&items[ITEM_AMETHYST],  "Amethyst",  "Medium gemstone", 50);
    init_item(&items[ITEM_RUBY],      "Ruby",      "Expensive gemstone", 100);
}
