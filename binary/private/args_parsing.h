#ifndef CHIP8_EMU_ARGS_PARSING_H
#define CHIP8_EMU_ARGS_PARSING_H

#include "emulator.h"
#include "engine.h"

#define ARGS_DEFAULT_SCALE 8
#define ARGS_DEFAULT_BACKGROUND_COLOR 0x222222
#define ARGS_DEFAULT_SPRITE_COLOR 0x00BB00
#define ARGS_DEFAULT_SILENT_COLOR 0x000000
#define ARGS_DEFAULT_BUZZER_COLOR 0x555555
#define ARGS_DEFAULT_BUZZER_TONE 2000.0
#define ARGS_DEFAULT_CYCLES_PER_FRAME 30
#define ARGS_DEFAULT_LOAD_FLAG_REGISTERS 1
#define ARGS_DEFAULT_SAVE_FLAG_REGISTERS 1

typedef struct s_env
{
    char const *rom_path;
    enum e_emu_rom_type rom_type;
    long cycle_per_frame;
    long scale;
    uint64_t quirks;
    uint64_t emu_options;
    uint64_t engine_options;
    long background_color;
    long sprite_color;
    long silent_color;
    long buzzer_color;
    double buzzer_tone;
    int reset_user_flags;
    int dont_save_user_flags;
} t_env;

int parse_args(t_env *env, int argc, char const **argv);

#endif // CHIP8_EMU_ARGS_PARSING_H
