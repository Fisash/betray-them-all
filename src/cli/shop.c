#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/shop.h"
#include "cli/base.h"
#include "cli/print.h"
#include "cli/command_parser.h"

#include "core/shop_system.h"

static void print_shop_items(struct shop *shop, const struct item_info info[])
{
    uint8_t i, item_count;
    struct item *item;

    puts("Shop items:");
    for(i = 0, item_count = 0; i < SHOP_MAX_ITEMS_COUNT; i++)
    {
        item = &shop->items[i];
        if(!item || item->id == ITEM_NONE)
            continue;

        printf("%d. %s (price: %d g.)\n", (++item_count), 
                                    info[item->id].title, 
                        shop_get_sell_price(shop, item));
    }
}

static void print_squad_items(struct squad *squad, struct shop *shop)
{
    uint8_t i, item_count;
    struct item *item;

    puts("Squad items:");
    for(i = 0, item_count = 0; i < SQUAD_MAX_ITEMS; i++)
    {
        item = &squad->inventory[i];
        if(!item || item->id == ITEM_NONE)
            continue;

        printf("%d. %s (price: %d g.)\n", (++item_count), 
                       squad->items_info[item->id].title, 
                         shop_get_buy_price(shop, item));
    }
}

static void cli_try_buy(struct parsed_command *cmd, 
            struct shop *shop, struct squad *squad)
{
    enum shop_transaction_status status;

    uint16_t item_num = cmd->args.shop_item_num;
    struct item *item = shop_get_item_by_num(shop, item_num);
    status = shop_system_try_buy_item(shop, item, squad);

    switch (status)
    {
        case SHOP_TRANSACTION_OK:
            puts("The squad bought this item!");
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_GOLD:
            puts("Squad doesn`t have enough gold!");
            break;
        case SHOP_TRANSACTION_INCORRECT_ITEM:
            puts("Incorrect item selection");
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_SPACE:
            puts("There is no free space in squad`s inventory");
            break;
    }
}

static void cli_try_sell(struct parsed_command *cmd, 
             struct shop *shop, struct squad *squad)
{
    enum shop_transaction_status status;

    uint16_t item_num = cmd->args.item_num; 
    struct item *item = squad_get_item_by_num(squad, item_num);
    status = shop_system_try_sell_item(shop, item, squad);

    switch (status)
    {
        case SHOP_TRANSACTION_OK:
            puts("The squad sold this item to a merchant!");
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_GOLD:
            puts("The merchant doesn`t have enough gold to buy this good from us");
            break;
        case SHOP_TRANSACTION_INCORRECT_ITEM:
            puts("Incorrect item selection");
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_SPACE:
            puts("The merchant doesn`t have enough space to store this good");
            break;
    }
}

static void cli_item_info(struct parsed_command *cmd, struct shop *shop,
                                    const struct item_info items_info[])
{
    uint8_t item_num = cmd->args.shop_item_num;
    struct item *item = shop_get_item_by_num(shop, item_num);
    if(item && item->id != ITEM_NONE)
        print_item_info(item, items_info, NULL);
}

static void print_squad_gold(const struct squad *squad)
{
    printf("Squad`s gold: %d\n", squad->gold);
}

void interpret_shop_cmd(struct parsed_command *cmd,
                         struct shop **active_shop, 
                               struct squad *squad)
{
    if (!cmd->is_correct)
    {
        puts(cmd->info->help_message);
        return;
    }
    
    switch(cmd->type)
    {
        case CMD_EXIT:
            *active_shop = NULL;
            break;
        case CMD_SHOP_BUY:
            cli_try_buy(cmd, *active_shop, squad);
            break;
        case CMD_SHOP_SELL:
            cli_try_sell(cmd, *active_shop, squad);
            break;
        case CMD_SQUAD_INFO_INV:
            print_squad_items(squad, *active_shop);
            print_squad_gold(squad);
            break;
        case CMD_SHOP_INFO_INV:
            print_shop_items(*active_shop, squad->items_info);
            break;
        case CMD_SHOP_INFO_ITEM:
            cli_item_info(cmd, *active_shop, squad->items_info);
            break;
        default:
            break;
    }
}

void cli_shop_run(struct command_inputer *inputer, 
                        struct shop **active_shop, 
                              struct squad *squad)
{
    while(*active_shop != NULL)
    {
        command_inputer_input(inputer);
        interpret_shop_cmd(&inputer->cmd, active_shop, squad);
    }
}
