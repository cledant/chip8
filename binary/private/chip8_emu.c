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
open_renderer(uint32_t scale,
              uint32_t background_color,
              uint32_t sprite_color,
              uint32_t silent_color,
              uint32_t buzzer_color,
              char const **err)
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
    renderer_set_colors(
      background_color, sprite_color, silent_color, buzzer_color);
    return (0);
}

int
main(int argc, char const **argv)
{
    t_env env = { NULL,
                  EMU_RT_CHIP_8_MODERN,
                  ARGS_DEFAULT_CYCLES_PER_FRAME,
                  ARGS_DEFAULT_SCALE,
                  0,
                  0,
                  0,
                  ARGS_DEFAULT_BACKGROUND_COLOR,
                  ARGS_DEFAULT_SPRITE_COLOR,
                  ARGS_DEFAULT_SILENT_COLOR,
                  ARGS_DEFAULT_BUZZER_COLOR,
                  ARGS_DEFAULT_BUZZER_TONE };
    if (parse_args(&env, argc, argv)) {
        return (1);
    }

    char const *err = NULL;
    if (emu_load_rom(
          env.rom_path, env.rom_type, env.quirks, env.emu_options, &err)) {
        shutdown(err);
        return (1);
    }
    if (open_renderer(env.scale,
                      env.background_color,
                      env.sprite_color,
                      env.silent_color,
                      env.buzzer_color,
                      &err)) {
        shutdown(err);
        return (1);
    }
    if (input_init(&err)) {
        shutdown(err);
        return (1);
    }
    if (audio_init(&err) || audio_set_buzzer_params(env.buzzer_tone, 1, &err)) {
        shutdown(err);
        return (1);
    }
    engine_init(env.cycle_per_frame, env.engine_options);
    engine_loop();
    shutdown(NULL);
    return (0);
}