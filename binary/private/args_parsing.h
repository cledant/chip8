#ifndef CHIP8_EMU_ARGS_PARSING_H
#define CHIP8_EMU_ARGS_PARSING_H

#include "emulator.h"

#define ARGS_DEFAULT_SCALE 8
#define ARGS_DEFAULT_BACKGROUND_COLOR 0x222222
#define ARGS_DEFAULT_SPRITE_COLOR 0x00BB00
#define ARGS_DEFAULT_SILENT_COLOR 0x000000
#define ARGS_DEFAULT_BUZZER_COLOR 0x555555
#define ARGS_DEFAULT_BUZZER_TONE 2000.0
#define ARGS_DEFAULT_CYCLES_PER_FRAME 30

typedef struct s_env
{
    char const *rom_path;
    enum e_emu_rom_type rom_type;
    long cycle_per_frame;
    long scale;
    uint64_t quirks;
    uint64_t options;
    long background_color;
    long sprite_color;
    long silent_color;
    long buzzer_color;
    double buzzer_tone;
} t_env;

int parse_args(t_env *env, int argc, char const **argv);

#endif // CHIP8_EMU_ARGS_PARSING_H
