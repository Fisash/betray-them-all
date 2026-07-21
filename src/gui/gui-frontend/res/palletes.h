#ifndef PALLETES_H
#define PALLETES_H

#include <stdint.h>

enum pallete_indexing {
    INDEX_BY_4BIT
};

enum pallete_id {
    PALLETE_MAIN,
    PALLETE_COUNT
};

struct pallete {
    uint8_t color_count;
    const uint32_t *data;
};

void palletes_load(struct pallete palletes[]);

#endif
