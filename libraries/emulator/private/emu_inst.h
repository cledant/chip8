#ifndef CHIP8_EMU_EMU_INST_H
#define CHIP8_EMU_EMU_INST_H

#include <stdint.h>

/*
 * Structs
 */
typedef struct s_emu_inst_nibbles
{
    char n1 : 4;
    char n2 : 4;
    char n3 : 4;
    char n4 : 4;
} emu_inst_nibbles_t;

typedef struct s_emu_inst
{
    union
    {
        uint16_t raw_data;
        emu_inst_nibbles_t nibbles;
    };
} emu_inst_t;

/*
 * Types
 */
typedef int (*emu_exec_fct)(emu_inst_t);
typedef int (*emu_parse_fct)(emu_inst_t);

#endif // CHIP8_EMU_EMU_INST_H
