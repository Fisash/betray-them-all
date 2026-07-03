#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

/* struct buffers sizes*/
#define EVENT_TITLE_BUF_SIZE 32
#define EVENT_MESSAGE_BUF_SIZE 128
#define EVENT_MAX_ANSWERS 4

/*action IDs for anwers action*/
typedef enum {
    ACTION_NONE = 0,
    ACTION_START_ANIMALS_BATTLE
} event_action_t;

typedef enum {
    EVENT_NONE = -1,
    EVENT_EXPLORED,
    EVENT_TEST_MEADOW,
    EVENT_TEST_FOREST,
    EVENT_FOREST_ANIMALS,
    EVENT_TEST_MOUNTAIN,
    EVENT_COUNT
} event_id_t;

typedef struct {
    uint8_t is_valid;
    char text[EVENT_TITLE_BUF_SIZE];
    event_action_t action_id;
    event_id_t rolling_event_id;
} event_answer_t;

typedef struct {
    char title[EVENT_TITLE_BUF_SIZE];
    char message[EVENT_MESSAGE_BUF_SIZE];
    uint64_t tags; /* bit mask */
    uint8_t weight;
    event_answer_t answers[EVENT_MAX_ANSWERS];
} event_t;


uint8_t event_get_answer_count(const event_t *event);

void event_init(event_t *event, const char *title, const char *message, 
                                         uint64_t tags, uint8_t weight,
                             event_answer_t answers[EVENT_MAX_ANSWERS]);

#endif
