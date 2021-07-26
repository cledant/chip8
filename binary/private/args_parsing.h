#ifndef CHIP8_EMU_ARGS_PARSING_H
#define CHIP8_EMU_ARGS_PARSING_H

#include "emulator.h"

#define DEFAULT_SCALE 8

typedef struct s_env
{
    char const *rom_path;
    enum e_emu_rom_type rom_type;
    long cycle_per_frame;
    long scale;
    int wrap;
    int bxnn_inst;
} t_env;

int parse_args(t_env *env, int argc, char const **argv);

#endif // CHIP8_EMU_ARGS_PARSING_H
