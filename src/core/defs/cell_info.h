#ifndef CELL_INFO_H
#define CELL_INFO_H

#include <stdint.h>

#define CELL_INFO_NAME_BUF_SIZE 32
#define CELL_INFO_DESCRIPTION_BUF_SIZE 128

enum cell_type_id {
    CELL_TYPE_NONE = 0,
    CELL_TYPE_MEADOW,
    CELL_TYPE_FOREST,
    CELL_TYPE_MOUNTAIN,
    CELL_TYPE_VILLAGE,
    CELL_TYPE_COUNT
};

struct cell_info {
    char title[CELL_INFO_NAME_BUF_SIZE];
    char description[CELL_INFO_DESCRIPTION_BUF_SIZE];
    uint8_t exploring_cd;
};

void cells_info_load(struct cell_info cells[]);

#endif
