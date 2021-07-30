#include "args_parsing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

typedef int (*args_parse_t)(void *,
                            int *,
                            int,
                            char const **,
                            char const *,
                            uint64_t const *);

static void
show_help()
{
    puts("./chip8_emu [-h | --help]"
         "[-b BACKGROUND_COLOR]"
         "[-c CYCLE_PER_FRAME]"
         "[-i SILENT_COLOR]"
         "[-p SPRITE_COLOR]"
         "[-s SCALE]"
         "[-t ROME_TYPE]"
         "[-u BUZZER_TONE]"
         "[-z BUZZER_ACTIVE_COLOR]"
         "[-A][-B][-W] ROM_FILEPATH");
    printf("\t-b: Background RGB color in hex format. Default is 0x%6x\n",
           ARGS_DEFAULT_BACKGROUND_COLOR);
    puts("\t-c: Cycles per frames (Emulation speed). Default is 30.");
    printf("\t-i: Silent RGB color in hex format. Default is 0x%06x\n",
           ARGS_DEFAULT_SILENT_COLOR);
    printf("\t-p: Sprite RGB color in hex format. Default is 0x%06x\n",
           ARGS_DEFAULT_SPRITE_COLOR);
    puts("\t-s: Resolution scaling from original resolution. Default is 8.");
    puts("\t-t: Rom Type can be CHIP8_COSMAC, CHIP8, SUPERCHIP8. Default is "
         "CHIP8.");
    printf("\t-u: Buzzer tone in Hz. Default is %g Hz\n",
           ARGS_DEFAULT_BUZZER_TONE);
    printf("\t-z: Buzzer RGB color in hex format. Default is 0x%6x\n",
           ARGS_DEFAULT_BUZZER_COLOR);
    puts("\t-A: Warns when instruction are not even aligned.");
    puts("\t-B: Activate SUPERCHIP8 quirk on BNNN instrution.");
    puts("\t-W: Change sprite drawing behaviour from clipping to wrapping.");
}

static int
parse_help(void *var,
           int *cur_arg,
           int max_args,
           char const **argv,
           char const *err_msg,
           uint64_t const *other)
{
    (void)var;
    (void)cur_arg;
    (void)max_args;
    (void)argv;
    (void)err_msg;
    (void)other;
    show_help();
    return (1);
}

static int
parse_emu_type(void *var,
               int *cur_arg,
               int max_args,
               char const **argv,
               char const *err_msg,
               uint64_t const *other)
{
    (void)other;
    if (!var || !argv || !cur_arg) {
        puts("chip8_emu: empty ptr when parsing emu type");
        return (1);
    }
    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }

    emu_rom_type_t *type = var;
    for (uint32_t i = 1; i < EMU_RT_NB_TYPES; ++i) {
        if (!strcmp(g_emu_rom_types_str[i], argv[*cur_arg + 1])) {
            *type = i;
            ++*cur_arg;
            return (0);
        }
    }
    if (err_msg) {
        puts(err_msg);
    } else {
        printf("chip8_emu: failed to parse: %s\n", argv[*cur_arg + 1]);
    }
    return (1);
}

static int
parse_long(void *var,
           int *cur_arg,
           int max_args,
           char const **argv,
           char const *err_msg,
           uint64_t const *other)
{
    if (!var || !other || !cur_arg || !argv) {
        puts("chip8_emu: empty ptr when parsing long");
        return (1);
    }
    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }

    long *ptr = var;
    uint64_t const *base = other;
    char *end_ptr;
    *ptr = strtol(argv[*cur_arg + 1], &end_ptr, *base);
    if (*end_ptr != '\0' && err_msg) {
        puts(err_msg);
        return (1);
    } else if (*end_ptr != '\0') {
        printf("chip8_emu: failed to parse: %s\n", argv[*cur_arg + 1]);
        return (1);
    }
    ++*cur_arg;
    return (0);
}

static int
parse_double(void *var,
             int *cur_arg,
             int max_args,
             char const **argv,
             char const *err_msg,
             uint64_t const *other)
{
    (void)other;
    if (!var || !cur_arg || !argv) {
        puts("chip8_emu: empty ptr when parsing long");
        return (1);
    }
    if (*cur_arg + 1 >= max_args) {
        *cur_arg = max_args;
        return (1);
    }

    double *ptr = var;
    char *end_ptr;
    *ptr = strtod(argv[*cur_arg + 1], &end_ptr);
    if (*end_ptr != '\0' && err_msg) {
        puts(err_msg);
        return (1);
    } else if (*end_ptr != '\0') {
        printf("chip8_emu: failed to parse: %s\n", argv[*cur_arg + 1]);
        return (1);
    }
    ++*cur_arg;
    return (0);
}

static int
parse_bit_field(void *var,
                int *cur_arg,
                int max_args,
                char const **argv,
                char const *err_msg,
                uint64_t const *other)
{
    if (!var || !other) {
        puts("chip8_emu: empty ptr when parsing wrap");
        return (1);
    }
    (void)cur_arg;
    (void)max_args;
    (void)argv;
    (void)err_msg;
    uint64_t *ptr = var;
    *ptr |= *other;
    return (0);
}

static int
error_checks(t_env const *env, char const **argv)
{
    if (env->rom_type == EMU_RT_NONE || env->rom_type >= EMU_RT_NB_TYPES) {
        printf("chip8_emu: Incorrect ROM type: %s\nROM TYPE can be "
               "CHIP8_COSMAC, CHIP8, SUPERCHIP8\n",
               argv[2]);
        return (1);
    }
    if (env->cycle_per_frame < 1) {
        puts("chip8_emu: CYCLE_PER_FRAME can't be 0 or inferior");
        return (1);
    }
    if (env->scale < 1) {
        puts("chip8_emu: SCALE can't be 0 or inferior");
        return (1);
    }
    if (env->background_color < 0x0 || env->background_color > 0xFFFFFF) {
        puts(
          "chip8_emu: BACKGROUND_COLOR must be between 0x000000 and 0xFFFFFF");
        return (1);
    }
    if (env->sprite_color < 0x0 || env->sprite_color > 0xFFFFFF) {
        puts("chip8_emu: SPRITE_COLOR must be between 0x000000 and 0xFFFFFF");
        return (1);
    }
    if (env->silent_color < 0x0 || env->silent_color > 0xFFFFFF) {
        puts("chip8_emu: SILENT_COLOR must be between 0x000000 and 0xFFFFFF");
        return (1);
    }
    if (env->buzzer_color < 0x0 || env->buzzer_color > 0xFFFFFF) {
        puts("chip8_emu: BUZZER_COLOR must be between 0x000000 and 0xFFFFFF");
        return (1);
    }
    if (env->buzzer_tone <= 0.0) {
        puts("chip8_emu: BUZZER_TONE can't be 0 or inferior");
        return (1);
    }
    return (0);
}

int
parse_args(t_env *env, int argc, char const **argv)
{
    /*
     * Parsing related arrays
     */
    static char const *options[] = { "-h", "--help", "-t", "-c", "-s",
                                     "-W", "-B",     "-u", "-b", "-p",
                                     "-i", "-z",     "-A" };
    static args_parse_t const fct_ptr[] = {
        parse_help,     parse_help,      parse_emu_type,  parse_long,
        parse_long,     parse_bit_field, parse_bit_field, parse_double,
        parse_long,     parse_long,      parse_long,      parse_long,
        parse_bit_field
    };

    /*
     * Fct Ptr args arrays
     */
    void *ptrs[] = {
        NULL,
        NULL,
        &env->rom_type,
        &env->cycle_per_frame,
        &env->scale,
        &env->quirks,
        &env->quirks,
        &env->buzzer_tone,
        &env->background_color,
        &env->sprite_color,
        &env->silent_color,
        &env->buzzer_color,
        &env->options,
    };
    static char const *err_msg[] = {
        NULL,
        NULL,
        "chip8_emu: ROM_TYPE is invalid",
        "chip8_emu: CYCLE_PER_FRAME is not a number",
        "chip8_emu: SCALE is not a number",
        NULL,
        NULL,
        "chip8_emu: BUZZER_TONE is not a number",
        "chip8_emu: BACKGROUND_COLOR is not a number",
        "chip8_emu: SPRITE_COLOR is not a number",
        "chip8_emu: SILENT_COLOR is not a number",
        "chip8_emu: BUZZER_COLOR is not a number",
        NULL
    };
    static uint64_t const others[] = {
        0, 0,  0,  10, 10, EMU_QUIRK_DRAW_WRAP,        EMU_QUIRK_BXNN_INST,
        0, 16, 16, 16, 16, EMU_OPTION_WARN_NOT_ALIGNED
    };

    if (argc == 1) {
        show_help();
        return (1);
    }
    for (int i = 1; i < argc - 1; ++i) {
        int arg_found = 0;
        for (uint32_t j = 0; j < ARRAY_SIZE(fct_ptr); ++j) {
            if (!strcmp(options[j], argv[i])) {
                arg_found = 1;
                if (fct_ptr[j](
                      ptrs[j], &i, argc, argv, err_msg[j], &others[j])) {
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
    if (error_checks(env, argv)) {
        return (1);
    }
    env->rom_path = argv[argc - 1];
    return (0);
}
