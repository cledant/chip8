#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "emulator.h"
#include "engine.h"
#include "args_parsing.h"

static void
shutdown(char const *msg)
{
    if (msg) {
        printf("chip8_emu: %s\n", msg);
    }
    renderer_destroy_window();
    input_destroy();
    audio_destroy();
    renderer_destroy();
}

static int
open_renderer(int32_t scale, char const **err)
{
    if (renderer_init(err)) {
        return (1);
    }
    int32_t fb_w = 0;
    int32_t fb_h = 0;
    if (emu_get_framebuffer_size(&fb_w, &fb_h)) {
        *err = "Unknown type of chip";
        return (1);
    }
    if (renderer_create_window(fb_w * scale, fb_h * scale, scale * 2, err)) {
        return (1);
    }
    if (renderer_create_framebuffer(fb_w, fb_h, err)) {
        return (1);
    }
    return (0);
}

int
main(int argc, char const **argv)
{
    t_env env = { NULL,
                  EMU_RT_CHIP_8_MODERN,
                  ENGINE_DEFAULT_CYCLES_PER_FRAME,
                  DEFAULT_SCALE,
                  0,
                  0 };
    if (parse_args(&env, argc, argv)) {
        return (1);
    }

    char const *err = NULL;
    if (emu_load_rom(env.rom_path, env.rom_type, &err)) {
        shutdown(err);
        return (1);
    }
    if (open_renderer(env.scale, &err)) {
        shutdown(err);
        return (1);
    }
    if (input_init(&err)) {
        shutdown(err);
        return (1);
    }
    if (audio_init(&err) || audio_set_buzzer_params(100, 1, &err)) {
        shutdown(err);
        return (1);
    }
    engine_init(env.cycle_per_frame);
    engine_loop();
    shutdown(NULL);
    return (0);
}