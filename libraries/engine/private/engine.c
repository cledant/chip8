#include "engine.h"

#include <stddef.h>

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
engine_loop(char const **err)
{
    while (!should_close) {
        if (input_get(err)) {
            return (1);
        }
        process_event();
        emu_execute(NULL);
    }
    return (0);
}
