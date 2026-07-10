#ifndef CELL_INFO_H
#define CELL_INFO_H

#define CELL_INFO_NAME_BUF_SIZE 32
#define CELL_INFO_DESCRIPTION_BUF_SIZE 128

typedef enum {
    CELL_TYPE_NONE = 0,
    CELL_TYPE_MEADOW,
    CELL_TYPE_FOREST,
    CELL_TYPE_MOUNTAIN,
    CELL_TYPE_VILLAGE,
    CELL_TYPE_COUNT
} cell_type_id_t;

typedef struct {
    char title[CELL_INFO_NAME_BUF_SIZE];
    char description[CELL_INFO_DESCRIPTION_BUF_SIZE];
} cell_info_t;

void cells_info_load(cell_info_t cells[]);

#endif
