#include "gui/gui-frontend/layout_node.h"

void layout_node_init(struct layout_node *node, 
                      enum alignment a_x, uint16_t margin_x,
                      enum alignment a_y, uint16_t margin_y,
                      enum size_rule width_rule, float width_value,  
                      enum size_rule height_rule,float height_value, 
                                         struct layout_node *parent)
{
    node->pos_x.align = a_x;
    node->pos_x.margin = margin_x;

    node->pos_y.align = a_y;
    node->pos_y.margin = margin_y;

    node->width_layout.rule = width_rule;
    node->width_layout.value = width_value;

    node->height_layout.rule = height_rule;
    node->height_layout.value = height_value;

    node->parent = parent;
}

static void calc_size_scalar(uint16_t *out, uint16_t parent_scalar, 
                                          struct layout_size *size)
{
    switch(size->rule)
    {
        case SIZE_ABSOLUTE_PIXELS:
        *out = (uint16_t)size->value;
        break;
        case SIZE_PARENT_NODE_PERCENT:
        *out = (uint16_t)(parent_scalar * (size->value / 100.0f));
        break;
        case SIZE_CUSTOM:
        default:
        break;
    }
}

static void calc_pos_scalar(uint16_t *out, uint16_t parent_pos_scalar,
               uint16_t parent_size_scalar, uint16_t node_size_scalar,
                                                struct layout_pos pos) 
{
    switch(pos.align)
    {
        case ALIGN_START:
        *out = parent_pos_scalar + pos.margin;
        break;
        case ALIGN_CENTER:
        *out = (parent_pos_scalar + 
               (parent_size_scalar - node_size_scalar)/2);
        break;
        case ALIGN_END:
        *out = (parent_pos_scalar + 
               (parent_size_scalar - node_size_scalar)) - pos.margin;
        break;
        default:
        break;
    }

}

void layout_node_calc(struct layout_node *n)
{
    if(!n || !n->parent)
        return;

    calc_size_scalar(&n->out.width, n->parent->out.width, 
                                       &n->width_layout);
    calc_size_scalar(&n->out.height, n->parent->out.height, 
                                        &n->height_layout);

    calc_pos_scalar(&n->out.x, n->parent->out.x, n->parent->out.width, 
                                              n->out.width, n->pos_x);
    calc_pos_scalar(&n->out.y, n->parent->out.y, n->parent->out.height, 
                                              n->out.height, n->pos_y);
}

void layout_node_update_root(struct layout_node *root, 
                                uint16_t window_width,
                               uint16_t window_height)
{
    root->out.x = 0;
    root->out.y = 0;
    root->out.width = window_width;
    root->out.height = window_height;
}
