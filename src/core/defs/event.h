#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

/* struct buffers sizes*/
#define EVENT_TITLE_BUF_SIZE 32
#define EVENT_MESSAGE_BUF_SIZE 128
#define EVENT_MAX_ANSWERS 4

/*action IDs for anwers action*/
enum event_action {
    ACTION_NONE = 0,
    ACTION_OPEN_VILLAGE_SHOP,
    ACTION_START_ANIMALS_BATTLE
};

enum event_id {
    EVENT_NONE = -1,
    EVENT_EXPLORED,
    EVENT_TEST_MEADOW,
    EVENT_TEST_FOREST,
    EVENT_FOREST_ANIMALS,
    EVENT_TEST_MOUNTAIN,
    EVENT_VILLAGE_MAIN,
    EVENT_COUNT
};

struct event_answer {
    uint8_t is_valid;
    char text[EVENT_TITLE_BUF_SIZE];
    enum event_action action_id;
    enum event_id rolling_event_id;
};

struct event {
    char title[EVENT_TITLE_BUF_SIZE];
    char message[EVENT_MESSAGE_BUF_SIZE];
    uint64_t tags; /* bit mask */
    uint8_t weight;
    struct event_answer answers[EVENT_MAX_ANSWERS];
};


uint8_t event_get_answer_count(const struct event *event);

void event_init(struct event *event, const char *title, const char *message, 
                                              uint64_t tags, uint8_t weight,
                             struct event_answer answers[EVENT_MAX_ANSWERS]);

#endif
