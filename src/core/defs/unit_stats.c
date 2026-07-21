#include <stdlib.h>

#include "core/defs/unit_stats.h"

void unit_stats_init(struct unit_stats *p, uint16_t strength, uint16_t agility, 
                                    uint16_t will, uint16_t intelligence)
{
    p->strength = strength;
    p->agility = agility;
    p->will = will;
    p->intelligence = intelligence;
}

uint16_t *unit_stats_get_stat(struct unit_stats *stats, enum stat_selection s)
{
    switch(s)
    {
        case STRENGTH: return &stats->strength;
        case AGILITY: return &stats->agility;
        case WILL: return &stats->will;
        case INTELLIGENCE: return &stats->intelligence;
        default: return NULL;
    }
}

int unit_stats_is_eligible(const struct unit_stats *unit, 
                           const struct unit_stats *req)
{
    return ((unit->strength >= req->strength)       &&
            (unit->agility >= req->agility)         &&
            (unit->will >= req->will)               &&
            (unit->intelligence >= req->intelligence));
}
