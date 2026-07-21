#ifndef UNIT_PROPS_H
#define UNIT_PROPS_H

#include <stdint.h>

struct unit_stats {
    uint16_t strength;
    uint16_t agility;
    uint16_t will;
    uint16_t intelligence;
};

enum stat_selection {
    STRENGTH,
    AGILITY,
    WILL,
    INTELLIGENCE
};

void unit_stats_init(struct unit_stats *p, uint16_t strength,
                     uint16_t agility, uint16_t will, uint16_t intelligence);

uint16_t *unit_stats_get_stat(struct unit_stats *stats, enum stat_selection s);

int unit_stats_is_eligible(const struct unit_stats *unit, 
                           const struct unit_stats *req);

#endif
