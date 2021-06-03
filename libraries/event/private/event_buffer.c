#include "event_buffer.h"

#define MAX_EVENT_NB 128

static event_t event_buffer[MAX_EVENT_NB];
static int32_t event_head = 0;
static int32_t event_tail = 0;

int
event_push(event_t const *event)
{
    event_buffer[event_head] = *event;
    event_head = (event_head + 1) % MAX_EVENT_NB;
    if (event_head == event_tail) {
        return (1);
    }
    return (0);
}

int
event_pop(event_t *event)
{
    if (event_head == event_tail) {
        return (1);
    }
    *event = event_buffer[event_tail];
    event_tail = (event_tail + 1) % MAX_EVENT_NB;
    return (0);
}
