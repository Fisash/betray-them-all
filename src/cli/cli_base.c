#include <stdio.h>
#include <string.h>

#include "cli/cli_base.h"

static void parse_command(char *input_buf, struct command *cmd)
{
    cmd->argc = 0;
    char* current_token = strtok(input_buf, ARGS_SPLITER);
    while (current_token != NULL && cmd->argc < MAX_AGRC)
    {
        cmd->argv[cmd->argc++] = current_token;
        current_token = strtok(NULL, ARGS_SPLITER);
    }
}

struct command cli_base_input_command(char input_buffer[])
{
    struct command result;
    do
    {
        memset(&result, 0, sizeof(result));
        fgets(input_buffer, INPUT_BUF_SIZE, stdin);
        parse_command(input_buffer, &result);
    }
    while (result.argc == 0);

    return result;
}

int cli_base_choose_number(uint8_t min, uint8_t max)
{
    int input;
    for(;;)
    {
        if(scanf("%d", &input) == 1)
        {
            if(input >= min && input <= max)
                return input;
        }
        else
            while(getc(stdin) != '\n');
    }
}

void print_stat_with_scaling(const char *label, int base_value,
               int real_value, const struct scaling_group *scalings,
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
