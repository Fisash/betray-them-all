#ifndef CLI_PRINT_H
#define CLI_PRINT_H
#include "core/state/game_state.h"

void print_unit_info(struct unit *unit, const struct item_info items[],
                     const struct unit_template templates[]);

void print_item_info(const struct item *item, const struct item_info info[],
                                                          struct unit *unit);

void print_stat_with_scaling(const char *label, int base_value, int real_value,
                             const struct scaling_group *scalings,
                             int is_show_real);

#endif
