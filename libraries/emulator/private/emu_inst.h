#ifndef CHIP8_EMU_EMU_INST_H
#define CHIP8_EMU_EMU_INST_H

#include <stdint.h>

/*
 * Defines
 */
#define EMU_CHIP8_NB_INST 35
#define EMU_SUPER_CHIP8_NB_INST 45

#define EMU_CHIP8_DRAW_INST 23
#define EMU_CHIP8_JP_VO_ADDR 21

#define EMU_SUPERCHIP8_JP_VO_ADDR 27
#define EMU_SUPERCHIP8_DRAW_EXTENDED_INST 29
#define EMU_SUPERCHIP8_DRAW_INST 30

/*
 * Structs
 */
typedef struct s_emu_inst
{
    uint8_t n1;
    uint8_t n2;
    uint8_t n3;
    uint8_t n4;
} emu_inst_t;

typedef struct s_emu_inst_addr
{
    uint8_t op_code;
    uint16_t addr;
} emu_inst_addr_t;

typedef struct s_emu_inst_reg_uint8
{
    uint8_t op_code;
    uint8_t gen_reg;
    uint8_t value;
} emu_inst_reg_uint8_t;

typedef struct s_emu_inst_reg_reg
{
    uint8_t op_code_1;
    uint8_t gen_reg_x;
    uint8_t gen_reg_y;
    uint8_t op_code_2;
} emu_inst_reg_reg_t;

typedef struct s_emu_inst_draw
{
    uint8_t op_code;
    uint8_t x;
    uint8_t y;
    uint8_t size;
} emu_inst_draw_t;

/*
 * Types
 */
typedef int (*emu_exec_fct_t)(emu_inst_t, void *, char const **);
typedef emu_exec_fct_t (*emu_parse_fct_t)(emu_inst_t);

/*
 * Conversion
 */
emu_inst_addr_t emu_inst_to_emu_inst_addr(emu_inst_t base);
emu_inst_reg_uint8_t emu_inst_to_emu_inst_reg_uint8(emu_inst_t base);

#endif // CHIP8_EMU_EMU_INST_H
