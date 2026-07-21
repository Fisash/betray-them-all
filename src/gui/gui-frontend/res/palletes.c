#include <stdlib.h>

#include "gui/gui-frontend/res/palletes.h"

static const uint32_t raw_main_data[] = {
    0xDDCF99, 0xCCA87B, 0xB97A60, 0x9C524E,
    0x774251, 0x4B3D44, 0x4E5463, 0x5B7D73,
    0x8E9F7D, 0x645355, 0x8C7C79, 0xA99C8D,
    0x7D7B62, 0xAAA25D, 0x846D59, 0xA88A5E
};

void palletes_load(struct pallete palletes[])
{
    struct pallete *p;

    p = &palletes[PALLETE_MAIN];
    p->color_count = 16;
    p->data = raw_main_data;
}
