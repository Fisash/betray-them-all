#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli/shop.h"
#include "cli/base.h"

#include "core/shop_system.h"

static void draw_shop_items(struct shop *shop, const struct item_info info[])
{
    uint8_t i, item_count;
    struct item *item;

    puts("Shop goods:");
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

static void draw_squad_items(struct squad *squad, struct shop *shop, 
                                 const struct item_info info[])
{
    uint8_t i, item_count;
    struct item *item;

    puts("Squad goods:");
    for(i = 0, item_count = 0; i < SQUAD_MAX_ITEMS; i++)
    {
        item = &squad->inventory[i];
        if(!item || item->id == ITEM_NONE)
            continue;

        printf("%d. %s (price: %d g.)\n", (++item_count), 
                                    info[item->id].title, 
                         shop_get_buy_price(shop, item));
    }
}

static void cli_try_buy(struct command *cmd, struct shop *shop, struct squad *squad)
{
    enum shop_transaction_status status;

    uint8_t num = (uint8_t)atoi(cmd->argv[1]);
    struct item *item = shop_get_item_by_num(shop, num);
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

static void cli_try_sell(struct command *cmd, struct shop *shop, struct squad *squad)
{
    enum shop_transaction_status status;

    uint8_t num = (uint8_t)atoi(cmd->argv[1]);
    struct item *item = squad_get_item_by_num(squad, num);
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

static void cli_item_info(struct command *cmd, struct shop *shop,
                                  const struct item_info items_info[])
{
    if(cmd->argc < 2) 
    {
        puts("Identify merchant`s item to put info");
        return;
    }
    
    uint8_t item_num = atoi(cmd->argv[1]);
    struct item *item = shop_get_item_by_num(shop, item_num);
    if(item && item->id != ITEM_NONE)
        print_item_info(item, items_info, NULL);
}

void cli_shop_run(struct shop **active_shop, struct squad *squad, 
                                   const struct item_info info[])
{
    int is_need_draw_shop_items = 1;
    char input_buf[INPUT_BUF_SIZE];
    struct command cmd;
    for(;;)
    {
        if(is_need_draw_shop_items)
        {
            printf("Squad gold: %d\n", squad->gold);
            draw_shop_items(*active_shop, info); 
        }

        cmd = cli_base_input_command(input_buf);

        if((strcmp(cmd.argv[0], "leave") == 0) || 
           (strcmp(cmd.argv[0], "quit") == 0)  ||
           (strcmp(cmd.argv[0], "q") == 0)      )
        {
            *active_shop = NULL;
            break;
        }

        is_need_draw_shop_items = 0;

        if(strcmp(cmd.argv[0], "inv") == 0)
            draw_squad_items(squad, *active_shop, info);

        if(strcmp(cmd.argv[0], "buy") == 0 && cmd.argc > 1)
        {
            cli_try_buy(&cmd, *active_shop, squad);
            is_need_draw_shop_items = 1;
        }

        if(strcmp(cmd.argv[0], "sell") == 0 && cmd.argc > 1)
        {
            cli_try_sell(&cmd, *active_shop, squad);
            is_need_draw_shop_items = 1;
        }

        if(strcmp(cmd.argv[0], "info") == 0 && cmd.argc > 1)
            cli_item_info(&cmd, *active_shop, info);
    }
}
