#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cli/terminal_view.h"

#include "core/world_queries.h"

void draw_context_init(struct draw_frame_context *context, struct world *world, 
                      struct squad *squad, const struct cell_info cells_info[], 
                                const struct item_info items_info[], int *days)
{
    context->world = world;
    context->squad = squad;
    context->cells_info = cells_info;
    context->items_info = items_info;
    context->days = days;
}

static char get_cell_view(char id)
{
    switch (id)
    {
        case CELL_TYPE_MEADOW:
            return CELL_VIEW_MEADOW;
            break;
        case CELL_TYPE_FOREST:
            return CELL_VIEW_FOREST;
            break;
        case CELL_TYPE_MOUNTAIN:
            return CELL_VIEW_MOUNTAIN;
            break;
        case CELL_TYPE_VILLAGE:
            return CELL_VIEW_VILLAGE;
            break;
        default:
            return CELL_VIEW_DEFAULT;
            break;
    }
}

static void view_cells_id(char *buf_ptr, size_t size)
{
    size_t i;
    for(i = 0; i < size; i++)
        buf_ptr[i] = get_cell_view(buf_ptr[i]); 
}

void terminal_view_init_framebuffer(char *framebuffer_ptr)
{
    memset(framebuffer_ptr, CELL_VIEW_DEFAULT, FRAME_WIDTH*FRAME_HEIGHT);
}

/* get offset from framebuffer by row and column*/
static char *get_cursor(char *framebuffer_ptr, uint16_t column, uint16_t row)
{
    return (framebuffer_ptr + (row * FRAME_WIDTH) + column);
}
    
static void draw_char(char **cursor_ptr, char c)
{
    **cursor_ptr = c;
    (*cursor_ptr)++;
}

static void draw_str(char **cursor_ptr, const char* str)
{
    while (*str != '\0')
    {
        **cursor_ptr = *str;
        (*cursor_ptr)++;
        str++;
    }
}

static void draw_int(char **cursor_ptr, int value)
{
    char buf[12];
    sprintf(buf, "%d", value);
    draw_str(cursor_ptr, (const char*)&buf);
}

static void draw_rect(char **cursor_ptr, const char *rect,
                      uint16_t rect_width, uint16_t rect_height)
{
    char *start_cursor = *cursor_ptr;
    int x, y;
    for(y = 0; y < rect_height; y++)
    {
        *cursor_ptr = start_cursor + (y*FRAME_WIDTH);
        for(x = 0; x < rect_width; x++)
        {
            **cursor_ptr = *rect;
            (*cursor_ptr)++;
            rect++;
        }
    }
}

/* in current implementation it needs specific world size
 * should make CAMERA struct for stored position and scale 
 * and rewrte for this*/
static void draw_world(char *framebuffer, struct world *world)
{
    char world_view[WORLD_HEIGHT][WORLD_WIDTH];

                    /* maybe just allocate .bss with this size? */
                    /* and after take pointer to that buf */

    world_fill_cells_id_buffer(world, (char*)&world_view);

    view_cells_id((char*)&world_view, WORLD_HEIGHT*WORLD_WIDTH); 

    char *cursor = get_cursor(framebuffer, 
                              WORLD_OFFSET_X, WORLD_OFFSET_Y);
    draw_rect(&cursor, (char*)&world_view, 
              WORLD_WIDTH, WORLD_HEIGHT);
}

static void draw_player(char *framebuffer, struct squad *squad)
{
    char *cursor = get_cursor(framebuffer,squad->pos_x+WORLD_OFFSET_X, 
                                          squad->pos_y+WORLD_OFFSET_Y);
    *cursor = CELL_VIEW_SQUAD;
}


static void draw_unit_info(char **cursor_ptr, struct unit *unit)
{
    draw_str(cursor_ptr, unit->name);
    draw_char(cursor_ptr, ':');
    draw_int(cursor_ptr, unit->hp);
    draw_char(cursor_ptr, '/');
    draw_int(cursor_ptr, unit->max_hp);
}

static void draw_squad_units(char *framebuffer, struct unit units[])
{
    char *cursor;
    int i, unit_id;
    unit_id = 0;
    for(i = 0; i < SQUAD_MAX_UNITS; i++)
    {
        if(!units[i].is_alive)
            continue;

        cursor = get_cursor(framebuffer, 56, 3+unit_id);
        draw_int(&cursor, unit_id+1);
        draw_char(&cursor, '.');
        draw_unit_info(&cursor, &units[i]);
        unit_id++;
    }
}

static void draw_squad_inventory(char *framebuffer, struct item inventory[],
                                        const struct item_info items_info[])
{
    char *cursor;
    int i, item_slot_id;
    item_slot_id = 0;
    for(i = 0; i < SQUAD_MAX_ITEMS; i++)
    {
        if(inventory[i].id == ITEM_NONE)
            continue;

        cursor = get_cursor(framebuffer, 56, 14+item_slot_id);
        draw_int(&cursor, item_slot_id+1);
        draw_char(&cursor, '.');
        draw_str(&cursor, items_info[inventory[i].id].title);
        item_slot_id++;
    }
}

static void draw_squad(char *framebuffer, struct squad *squad, 
                          const struct item_info items_info[])
{
    char *cursor= get_cursor(framebuffer, 60, 1);
    draw_str(&cursor, "||||SQUAD||||");
    cursor= get_cursor(framebuffer, 64, 2);
    draw_str(&cursor, "units:");
    draw_squad_units(framebuffer, squad->units);
    draw_squad_inventory(framebuffer, squad->inventory, items_info);
}



void draw_squad_cell_info(char **cursor_ptr, struct draw_frame_context *context)
{

    struct cell *cell = world_queries_get_squad_cell(context->squad, 
                                                     context->world);

    const struct cell_info *info = &context->cells_info[cell->type_id];

    draw_str(cursor_ptr, info->title); 
    draw_char(cursor_ptr, '(');
    draw_int(cursor_ptr, context->squad->pos_x);
    draw_char(cursor_ptr, ',');
    draw_int(cursor_ptr, context->squad->pos_y);
    draw_char(cursor_ptr, ')');
    if(cell->days_until_update > 0)
    {
        draw_str(cursor_ptr, " cd:");
        draw_int(cursor_ptr, cell->days_until_update); 
        draw_str(cursor_ptr, " d.");
    }
}

void terminal_view_update_framebuffer(char *framebuffer, 
                                      struct draw_frame_context *context)

{
    draw_world(framebuffer, context->world);

    uint16_t gold = context->squad->gold;
    uint16_t provision = context->squad->provision;

    char *cursor = get_cursor(framebuffer, 0, 0);
    draw_str(&cursor, "gold: ");
    draw_int(&cursor, gold);
    draw_str(&cursor, " provision: ");
    draw_int(&cursor, provision);
    draw_str(&cursor, "  day: ");
    draw_int(&cursor, *(context->days));

    cursor = get_cursor(framebuffer, 56, 0);
    draw_squad_cell_info(&cursor, context);

    draw_squad(framebuffer, context->squad, context->items_info);

    draw_player(framebuffer, context->squad);
}


void terminal_view_redraw(char *framebuffer, 
                          struct draw_frame_context *context)
{
    terminal_view_init_framebuffer(framebuffer);
    terminal_view_update_framebuffer(framebuffer, context);
}

static void clear_screen()
{
    printf("\033[2J\033[H");
}


void terminal_view_stdout_framebuffer(char *framebuffer)
{
    clear_screen();

    int x, y;
    for (y = 0; y < FRAME_HEIGHT; y++)
    {
        for (x = 0; x < FRAME_WIDTH; x++)
        {
            putchar(*framebuffer);
            framebuffer++;
        }
        putchar('\n');
    }
}
