#ifndef LAYOUT_NODE_H
#define LAYOUT_NODE_H

#include <stdint.h>

enum alignment {
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END
};

enum size_rule {
    SIZE_ABSOLUTE_PIXELS,
    SIZE_PARENT_NODE_PERCENT,
    SIZE_CUSTOM
};

struct layout_size {
    enum size_rule rule;
    float value;
};

struct layout_pos {
    enum alignment align;
    uint16_t margin;
};

struct layout_node_output {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

struct layout_node {
    struct layout_pos pos_x; 
    struct layout_pos pos_y; 
    struct layout_size width_layout;
    struct layout_size height_layout;
    
    struct layout_node *parent;

    struct layout_node_output out;
};

void layout_node_init(struct layout_node *node, 
                      enum alignment a_x, uint16_t padding_x,
                      enum alignment a_y, uint16_t padding_y,
                      enum size_rule width_rule, float width_value,  
                      enum size_rule height_rule,float height_value, 
                                        struct layout_node *parent);

void layout_node_calc(struct layout_node *n);

void layout_node_update_root(struct layout_node *root, 
                                uint16_t window_width,
                              uint16_t window_height);

#endif
