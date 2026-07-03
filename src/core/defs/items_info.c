#include <string.h>

#include "core/defs/items_info.h"

static void init_item(item_info_t *item, const char *title, 
                                   const char *description)
{
    item->type = ITEM_TYPE_GENERIC;
    strcpy(item->title, title);
    strcpy(item->description, description);
}

static void init_armor(item_info_t *armor, const char *title, 
                const char *description, uint16_t protection)
{
    init_item(armor, title, description);
    armor->type = ITEM_TYPE_ARMOR;
    armor->props.armor.protection = protection;
}

static void init_weapon(item_info_t *weapon, const char *title, 
                      const char *description, uint16_t damage)
{
    init_item(weapon, title, description);
    weapon->type = ITEM_TYPE_WEAPON;
    weapon->props.weapon.damage = damage;
}

void items_info_load(item_info_t items[])
{
    init_armor(&items[ARMOR_FABRIC],  "Fabric armor",
                       "Light armor from fabric", 2);
    init_armor(&items[ARMOR_LEATHER], "Leather armor",
                       "Light armor from leather", 4);
    init_armor(&items[ARMOR_IRON],    "Iron armor",
                          "Medium weight armor", 8);
    init_armor(&items[ARMOR_SHELL],   "Shell armor",
                              "Best protection", 12);

    init_weapon(&items[WEAPON_SHORT_SWORD], "Short sword",
                       "Small but light-weight sword", 5);
    init_weapon(&items[WEAPON_SWORD], "Sword",
          "Standart size and damage sword", 8);
    init_weapon(&items[WEAPON_DAGGER], "Dagger",
          "Comact but deadly dagger in skilled hands ", 4);
    init_weapon(&items[WEAPON_MACE], "Mace",
            "Fast and light-weight mace", 4);
    init_weapon(&items[WEAPON_HUMMER], "Hummer",
                         "Huge war hummer", 10);
    init_weapon(&items[WEAPON_SPEAR], "Spear",
                   "Long and simple spear", 6);
    init_weapon(&items[WEAPON_HALBERD], "Halberd",
                      "Powerful roal halberd", 9);

    init_item(&items[ITEM_MALACHITE], "Malachite", "Basic gemstone");
    init_item(&items[ITEM_AMETHYST],  "Amethyst",  "Medium gemstone");
    init_item(&items[ITEM_RUBY],      "Ruby",      "Expensive gemstone");
}
