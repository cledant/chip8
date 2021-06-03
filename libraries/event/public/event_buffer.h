#ifndef CHIP8_EMU_EVENT_H
#define CHIP8_EMU_EVENT_H

#include <stdint.h>

typedef enum e_event_type
{
    ET_NONE = 0,
    ET_SYS_CLOSE,
    ET_SYS_PAUSE,
    ET_KEY_PRESS,
    ET_KEY_RELEASE,
    ET_NB
} event_type_t;

typedef struct s_event
{
    event_type_t type;
    int32_t data1;
    int32_t data2;
} event_t;

int event_push(event_t const *event);
int event_pop(event_t *event);

#endif // CHIP8_EMU_EVENT_H
