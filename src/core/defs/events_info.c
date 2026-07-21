#include <string.h>

#include "core/defs/events_info.h"

static void answers_clear(struct event_answer *answers)
{
    int i;
    for(i = 0; i < EVENT_MAX_ANSWERS; i++)
        answers[i].is_valid = 0;
}

static void answer_init(struct event_answer *answer, const char *text, 
                     uint16_t action_id, int16_t rolling_event_id)
{
    answer->is_valid = 1;
    strcpy(answer->text, text);
    answer->action_id = action_id;
    answer->rolling_event_id = rolling_event_id;
}

static void calculate_weight_sums(struct events_info *events_info)
{
    uint16_t *sums_array = events_info->cell_type_weight_sums;
    memset(sums_array, 0, sizeof(uint16_t)*CELL_TYPE_COUNT);

    struct event *event;
    int event_i, cell_type_i;
    uint64_t type_mask;
    for (event_i = 0; event_i < EVENT_COUNT; event_i++)
    {
        event = &events_info->events[event_i];
        for(cell_type_i = 1; cell_type_i < CELL_TYPE_COUNT; cell_type_i++)
        {
            type_mask = (uint64_t)1<<cell_type_i;
            if(event->tags & type_mask)
                sums_array[cell_type_i] += event->weight;
        }
    }
}

void events_info_load(struct events_info *load_ptr)
{
    struct event_answer answers[EVENT_MAX_ANSWERS];

    /*EVENT EXPLORED*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "will look something else", 
                                            ACTION_NONE,
                                            EVENT_NONE);
    event_init(&load_ptr->events[EVENT_EXPLORED], 
        "Already explored", 
        "The cell has already been explored by our squad recently. There`s nothing interesting here for a while",  
        0, 
        0, 
        answers);

    /*EVENT_TEST_MEADOW*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "ok", 
                        ACTION_NONE,
                        EVENT_NONE);
    event_init(&load_ptr->events[EVENT_TEST_MEADOW], 
        "Meadow", 
        "Squad wandered through the meadow but did not finded anything intresting",  
        (uint64_t)1<<TAG_MEADOW, 
        10, 
        answers);

    /*EVENT_TEST_FOREST*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "ok", 
                          ACTION_NONE,
                          EVENT_NONE);
    event_init(&load_ptr->events[EVENT_TEST_FOREST], 
        "Forest", 
        "The forest is quite quiet. Nothing dangerous or cursious",  
        (uint64_t)1<<TAG_FOREST, 
        10,
        answers);

    /*EVENT_FOREST_ANIMALS*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "we have to fight", 
                          ACTION_START_ANIMALS_BATTLE,
                          EVENT_NONE);
    event_init(&load_ptr->events[EVENT_FOREST_ANIMALS], 
        "Forest", 
        "Don`t you thinks there are scary animal faces looking at us?",  
        (uint64_t)1<<TAG_FOREST, 
        25,
        answers);

    /*EVENT_TEST_MOUNTAIN*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "ok", 
                          ACTION_NONE,
                          EVENT_NONE);
    event_init(&load_ptr->events[EVENT_TEST_MOUNTAIN], 
        "Mountain", 
        "test text",  
        (uint64_t)1<<TAG_MOUNTAIN, 
        5,
        answers);

    /*EVENT_VILLAGE_MAIN*/
    answers_clear((struct event_answer*)&answers);
    answer_init(&answers[0], "Go to shop", 
                          ACTION_OPEN_VILLAGE_SHOP,
                          EVENT_NONE);
    answer_init(&answers[1], "Leaving the village", 
                          ACTION_NONE,
                          EVENT_NONE);
    event_init(&load_ptr->events[EVENT_VILLAGE_MAIN], 
        "Village", 
        "Our squad is on the main village street. Where should we go?",  
        (uint64_t)1<<TAG_VILLAGE, 
        10,
        answers);

    calculate_weight_sums(load_ptr);
}
