#include "args_parsing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*args_parse_t)(t_env *, int *, int, char const **);

static void
show_help()
{
    puts("./chip8_emu [-h | --help][-t ROME_TYPE][-s SCALE][-c "
         "CYCLE_PER_FRAME][-wB] ROM_FILEPATH");
    puts("\t-t: Rom type can be CHIP8_COSMAC, CHIP8, SUPERCHIP8. Default is "
         "CHIP8.");
    puts("\t-c: Emulation speed. Default is 30.");
    puts("\t-s: Resolution scaling from original resolution. Default is 8.");
    puts("\t-w: Change sprite drawing behaviour from clipping to wrapping.");
    puts("\t-B: Activate superchip8 quirk on BNNN instrution.");
}

static int
parse_help(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    (void)env;
    (void)cur_arg;
    (void)max_args;
    (void)argv;
    show_help();
    return (1);
}

static int
parse_emu_type(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    static char const *types[] = {
        "NONE", "CHIP8", "SUPERCHIP8", "CHIP8_COSMAC"
    };

    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }
    for (uint32_t i = 1; i < EMU_RT_NB_TYPES; ++i) {
        if (!strcmp(types[i], argv[*cur_arg + 1])) {
            env->rom_type = i;
            ++*cur_arg;
            return (0);
        }
    }
    printf("chip8_emu: invalid ROM_TYPE: %s\n", argv[*cur_arg + 1]);
    return (1);
}

static int
parse_emu_speed(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }
    char *end_ptr;
    env->cycle_per_frame = strtol(argv[*cur_arg + 1], &end_ptr, 10);
    if (*end_ptr != '\0') {
        puts("chip8_emu: CYCLE_PER_FRAME is not a number");
        return (1);
    }
    ++*cur_arg;
    return (0);
}

static int
parse_scaling(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }
    char *end_ptr;
    env->scale = strtol(argv[*cur_arg + 1], &end_ptr, 10);
    if (*end_ptr != '\0') {
        puts("chip8_emu: SCALE is not a number");
        return (1);
    }
    ++*cur_arg;
    return (0);
}

static int
parse_wrap(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    (void)env;
    (void)cur_arg;
    (void)max_args;
    (void)argv;
    env->wrap = 1;
    return (1);
}

static int
parse_bxnn_inst(t_env *env, int *cur_arg, int max_args, char const **argv)
{
    (void)env;
    (void)cur_arg;
    (void)max_args;
    (void)argv;
    env->bxnn_inst = 1;
    return (1);
}

int
parse_args(t_env *env, int argc, char const **argv)
{
    static char const *options[] = { "-h", "--help", "-t", "-c",
                                     "-s", "-w",     "-B" };
    static args_parse_t const fct_ptr[] = { parse_help,     parse_help,
                                            parse_emu_type, parse_emu_speed,
                                            parse_scaling,  parse_wrap,
                                            parse_bxnn_inst };

    if (argc == 1) {
        show_help();
        return (1);
    }
    for (int i = 1; i < argc - 1; ++i) {
        int arg_found = 0;
        for (uint32_t j = 0; j < (sizeof(*options) - 1); ++j) {
            if (!strcmp(options[j], argv[i])) {
                arg_found = 1;
                if (fct_ptr[j](env, &i, argc, argv)) {
                    return (1);
                }
                break;
            }
        }
        if (!arg_found) {
            printf("chip8_emu: Unknown args: %s\n", argv[i]);
            return (1);
        }
    }
    if (env->rom_type == EMU_RT_NONE || env->rom_type >= EMU_RT_NB_TYPES) {
        printf("chip8_emu: Incorrect ROM type: %s\nROM TYPE can be "
               "CHIP8_COSMAC, CHIP8, SUPERCHIP8\n",
               argv[2]);
        return (1);
    }
    if (env->scale < 1) {
        puts("chip8_emu: SCALE can't be 0 or inferior");
        return (1);
    }
    env->rom_path = argv[argc - 1];
    return (0);
}
