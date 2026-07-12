#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "terminal-io/cli_shop.h"
#include "terminal-io/cli_base.h"

#include "core/shop_system.h"

static void draw_shop_items(shop_t *shop, const item_info_t info[])
{
    uint8_t i, item_count;
    item_t *item;

    puts("Goods for sale:");
    for(i = 0, item_count = 0; i < SHOP_MAX_ITEMS_COUNT; i++)
    {
        item = &shop->items[i];
        if(!item || item->id == ITEM_NONE)
            continue;

        printf("%d. %s (%d gold)\n", (++item_count), 
                               info[item->id].title, 
                    shop_get_buy_price(shop, item));
    }
}

static void cli_try_buy(command *cmd, shop_t *shop, squad_t *squad)
{
     
    shop_transaction_status_t status;
    uint8_t slot_index = (uint8_t)atoi(cmd->argv[1]);
    status = shop_system_try_buy_item_by_squad(shop, slot_index, squad);
    switch (status)
    {
        case SHOP_TRANSACTION_OK:
            printf("The purchase is complited! Squad gold:%d\n",
                                                    squad->gold);
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_GOLD:
            printf("No enough gold (squad only has %d)\n",
                                             squad->gold);
            break;
        case SHOP_TRANSACTION_INCORRECT_ITEM:
            puts("Incorrect item selection");
            break;
        case SHOP_TRANSACTION_INSUFFCIENT_SPACE:
            puts("No free space from inventory");
            break;
    }
}

void cli_shop_run(shop_t **active_shop, squad_t *squad, 
                              const item_info_t info[])
{
    char input_buf[INPUT_BUF_SIZE];
    command cmd;
    for(;;)
    {
        draw_shop_items(*active_shop, info); 
        cmd = cli_base_input_command(input_buf);

        if(strcmp(cmd.argv[0], "leave") == 0)
        {
            *active_shop = NULL;
            break;
        }

        if(strcmp(cmd.argv[0], "buy") == 0 && cmd.argc > 1)
            cli_try_buy(&cmd, *active_shop, squad);
    }
}
