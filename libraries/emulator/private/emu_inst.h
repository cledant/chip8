#ifndef CHIP8_EMU_EMU_INST_H
#define CHIP8_EMU_EMU_INST_H

#include <stdint.h>
#include <assert.h>

/*
 * Defines
 */
#define EMU_CHIP8_NB_INST 35
#define EMU_CHIP8_HI_RES_NB_INST 37
#define EMU_SUPER_CHIP8_NB_INST 39

/*
 * Structs
 */
#pragma pack(push, 2)
typedef struct s_emu_inst_nibbles
{
    uint8_t n1 : 4;
    uint8_t n2 : 4;
    uint8_t n3 : 4;
    uint8_t n4 : 4;
} emu_inst_nibbles_t;

typedef struct s_emu_inst_addr
{
    uint8_t op_code : 4;
    uint16_t addr : 12;
} emu_inst_addr_t;
#pragma pack(pop)
static_assert(sizeof(emu_inst_addr_t) == 2, "emu_inst_addr_t size isn't 2");

typedef union u_emu_inst
{
    uint16_t raw_data;
    emu_inst_nibbles_t nibbles;
    emu_inst_addr_t addr;
} emu_inst_t;
static_assert(sizeof(emu_inst_t) == 2, "emu_inst_t size isn't 2");

/*
 * Types
 */
typedef int (*emu_exec_fct_t)(emu_inst_t, void *, char const **);
typedef emu_exec_fct_t (*emu_parse_fct_t)(emu_inst_t);

#endif // CHIP8_EMU_EMU_INST_H
