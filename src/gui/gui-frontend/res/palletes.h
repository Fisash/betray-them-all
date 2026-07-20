#ifndef PALLETES_H
#define PALLETES_H

#include <stdint.h>

typedef enum {
    INDEX_BY_4BIT
} pallete_indexing_t;

typedef enum {
    PALLETE_MAIN,
    PALLETE_COUNT
} pallete_id_t;

typedef struct {
    uint8_t color_count;
    const uint32_t *data;
} pallete_t;

void palletes_load(pallete_t palletes[]);

#endif
