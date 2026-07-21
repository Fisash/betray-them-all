#include <string.h>

#include "core/defs/event.h"

uint8_t event_get_answer_count(const struct event *event)
{
    int i;
    uint8_t result = 0;
    for(i = 0; i < EVENT_MAX_ANSWERS; i++)
    {
        if(event->answers[i].is_valid)
            result++;
    }
    return result;
}

void event_init(struct event *event, const char *title, const char *message, 
                                         uint64_t tags, uint8_t weight,
                              struct event_answer answers[EVENT_MAX_ANSWERS])
{
    strcpy(event->title, title);
    strcpy(event->message, message);
    event->tags = tags;
    event->weight = weight;
    int i;
    for(i = 0; i < EVENT_MAX_ANSWERS; i++)
        event->answers[i] = answers[i];
}
