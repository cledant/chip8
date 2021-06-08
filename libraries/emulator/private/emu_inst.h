#ifndef CHIP8_EMU_EMU_INST_H
#define CHIP8_EMU_EMU_INST_H

#include <stdint.h>

/*
 * Defines
 */
#define EMU_CHIP8_NB_INST 33
#define EMU_CHIP8_HI_RES_NB_INST 35 // TODO : Check
#define EMU_SUPER_CHIP8_NB_INST 43  // TODO : Check

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
typedef int (*emu_exec_fct_t)(emu_inst_t, void *, char const **);
typedef emu_exec_fct_t (*emu_parse_fct_t)(emu_inst_t);

#endif // CHIP8_EMU_EMU_INST_H
