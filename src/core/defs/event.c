#include <string.h>

#include "core/defs/event.h"

uint8_t event_get_answer_count(const event_t *event)
{
    uint8_t result = 0;
    for(int i = 0; i < EVENT_MAX_ANSWERS; i++)
    {
        if(event->answers[i].is_valid)
            result++;
    }
    return result;
}

void event_init(event_t *event, const char *title, const char *message, 
                                         uint64_t tags, uint8_t weight,
                              event_answer_t answers[EVENT_MAX_ANSWERS])
{
    strcpy(event->title, title);
    strcpy(event->message, message);
    event->tags = tags;
    event->weight = weight;
    for(int i = 0; i < EVENT_MAX_ANSWERS; i++)
        event->answers[i] = answers[i];
}
