#include "engine.h"

#include <stdio.h>

#include "emulator.h"
#include "input.h"
#include "event_buffer.h"

static int should_close;

static int
process_event()
{
    event_t event = { ET_NONE, 0, 0 };

    while (!event_pop(&event)) {
        switch (event.type) {
            case ET_SYS_CLOSE:
                should_close = 1;
                break;
            case ET_KEY_PRESS:
                emu_press_key(event.data1);
                break;
            case ET_KEY_RELEASE:
                emu_release_key(event.data1);
                break;
            default:
                break;
        }
    }
    return (0);
}

int
engine_loop()
{
    char const *err = NULL;

    while (!should_close) {
        if (input_get(&err)) {
            printf("[FATAL][INPUT]: %s\n", err);
            return (1);
        }
        process_event();
        if (emu_fetch(&err)) {
            printf("[FATAL][FETCH]: %s\n", err);
            return (1);
        }
        if (emu_decode(&err)) {
            printf("[WARN][DECODE]: %s\n", err);
            err = NULL;
        }
        if (emu_execute(&err)) {
            printf("[WARN][EXEC]: %s\n", err);
            err = NULL;
        }
    }
    return (0);
}
