#ifndef CHIP8_EMU_EMU_STATE_H
#define CHIP8_EMU_EMU_STATE_H

#include <stdint.h>

#include "emu_fb.h"

/*
 * Ram Locations
 */
#define EMU_CHIP8_RAM_ENTRY_POINT 0x200
#define EMU_CHIP8_MAX_PROG_RAM_ADDR (0xE90 - 1)
#define EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR (0xFFF - 1)

/*
 * Sizes
 */
#define EMU_RAM_SIZE (0xFFF + 1)
#define EMU_MAX_ROM_SIZE (EMU_RAM_SIZE - EMU_CHIP8_RAM_ENTRY_POINT)
#define EMU_MAX_STACK_SIZE 16
#define EMU_MAX_GENERAL_REGISTERS 16
#define EMU_MAX_RPL_USER_FLAGS 8
#define EMU_NB_KEYS 16
#define EMU_CHIP_8_FONT_HEIGHT 5
#define EMU_NB_FONTS 16

/*
 * Structs
 */
typedef struct s_emu_registers_state
{
    uint8_t general_registers[EMU_MAX_GENERAL_REGISTERS];
    uint8_t rpl_user_flags[EMU_MAX_RPL_USER_FLAGS];
    uint16_t address_register;
    uint16_t program_counter;
    uint16_t stack_pointer[EMU_MAX_STACK_SIZE];
    uint8_t delay_register;
    uint8_t sound_register;
    uint8_t current_stack;
} emu_registers_state_t;

typedef struct s_emu_state
{
    uint32_t max_addr;
    uint32_t max_fb;
    emu_registers_state_t registers;
    int32_t keys_state[EMU_NB_KEYS];
    uint8_t framebuffer[EMU_FRAMEBUFFER_MAX_SIZE];
    uint8_t ram[EMU_RAM_SIZE];
    uint8_t skip_fetch;
    uint8_t high_res_mode;
    uint8_t should_exit;
} emu_state_t;

#endif // CHIP8_EMU_EMU_STATE_H
