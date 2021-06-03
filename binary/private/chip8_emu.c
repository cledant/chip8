#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer.h"
#include "input.h"
#include "emulator.h"
#include "engine.h"

typedef struct s_env
{
    char const *rom_path;
    enum e_emu_rom_type rom_type;
    long scale;
} t_env;

static void
show_help()
{
    puts("./chip8_emu [-h | --help] ROM_FILEPATH ROM_TYPE [SCALE]");
    puts("\tROM TYPE can be CHIP8, CHIP8_HI_RES, SUPERCHIP8");
    puts("\tSCALE default is 4");
}

static void
shutdown(char const *msg)
{
    if (msg) {
        printf("chip8_emu: %s\n", msg);
    }
    renderer_destroy_window();
    input_destroy();
    renderer_destroy();
}

static int
parse_args(t_env *env, int argc, char const **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i])) {
            show_help();
            return (1);
        }
    }
    if (argc < 3 || argc > 5) {
        show_help();
        return (1);
    }
    for (int i = 0; i < EMU_RT_NB_TYPES; i++) {
        if (!strcmp(argv[2], emu_rom_types_str[i])) {
            env->rom_type = i;
        }
    }
    env->rom_path = argv[1];
    if (argc == 4) {
        char *end_ptr;

        env->scale = strtol(argv[3], &end_ptr, 10);
        if (*end_ptr != '\0') {
            puts("chip8_emu: SCALE is not a number");
            return (1);
        }
    }
    return (0);
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
    if (renderer_create_window(fb_w * scale, fb_h * scale, err)) {
        return (1);
    }
    if (renderer_create_framebuffer(fb_w, fb_h, 0x00AA00, err)) {
        return (1);
    }
    return (0);
}

int
main(int argc, char const **argv)
{
    t_env env = { NULL, EMU_RT_NONE, 4 };
    if (parse_args(&env, argc, argv)) {
        return (1);
    }
    if (env.rom_type == EMU_RT_NONE) {
        printf("chip8_emu: Incorrect ROM type: %s\nROM TYPE can be CHIP8, "
               "CHIP8_HI_RES, SUPERCHIP8\n",
               argv[2]);
        return (1);
    }
    if (env.scale <= 0) {
        puts("chip8_emu: SCALE can't be 0 or inferior");
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
    emu_set_draw_fct(&renderer_draw);
    if (input_init(&err)) {
        shutdown(err);
        return (1);
    }
    engine_loop(&err);
    shutdown(err);
    return (0);
}