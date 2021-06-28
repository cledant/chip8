#include "engine.h"

#include <stdio.h>

#include "emulator.h"
#include "input.h"
#include "event_buffer.h"
#include "renderer.h"
#include "audio.h"
#include "tools.h"

#define ENGINE_DRAW_RATE 60

/*
 * Engine variables
 */
static int eg_should_close;
static int eg_should_draw;
static int eg_should_cycle;
static double const eg_draw_tick = 1.0 / ENGINE_DRAW_RATE;
static double eg_cycle_tick =
  1.0 / (ENGINE_DEFAULT_CYCLES_PER_FRAME * ENGINE_DRAW_RATE);
static double eg_next_draw_timer;
static double eg_next_cycle_timer;

static int
process_event()
{
    event_t event = { ET_NONE, 0, 0 };

    while (!event_pop(&event)) {
        switch (event.type) {
            case ET_SYS_CLOSE:
                eg_should_close = 1;
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

static void
handle_timers()
{
    double cur_time = tool_get_time();

    if (cur_time >= eg_next_cycle_timer) {
        eg_next_cycle_timer = cur_time + eg_cycle_tick;
        eg_should_cycle = 1;
    }
    if (cur_time >= eg_next_draw_timer) {
        eg_next_draw_timer = cur_time + eg_draw_tick;
        eg_should_draw = 1;
    }
}

/*
 * Public API
 */
int
engine_init(uint32_t cycles_per_frame)
{
    eg_cycle_tick = 1 / (double)(cycles_per_frame * ENGINE_DRAW_RATE);
    eg_next_draw_timer = tool_get_time();
    eg_next_cycle_timer = tool_get_time();
    return (0);
}

int
engine_loop()
{
    char const *err = NULL;
    void const *emu_fb = emu_get_framebuffer();

    while (!eg_should_close) {
        if (input_get(&err)) {
            printf("[FATAL][INPUT]: %s\n", err);
            return (1);
        }
        process_event();
        handle_timers();
        if (eg_should_cycle) {
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
            eg_should_cycle = 0;
        }
        if (eg_should_draw) {
            emu_decrement_timers();
            int is_sound_active = emu_is_sound_active();
            (is_sound_active) ? audio_play_buzzer() : audio_stop_buzzer();
            renderer_draw(emu_fb, is_sound_active);
            eg_should_draw = 0;
        }
    }
    return (0);
}
