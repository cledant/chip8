#ifndef CHIP8_EMU_EMU_STATE_H
#define CHIP8_EMU_EMU_STATE_H

#include <stdint.h>

#include "emu_fb.h"

/*
 * Defines
 */
#define EMU_MAX_STACK_SIZE 16
#define EMU_MAX_GENERAL_REGISTERS 16
#define EMU_NB_KEYS 16

/*
 * Structs
 */
typedef struct s_emu_registers_state
{
    uint8_t general_registers[EMU_MAX_GENERAL_REGISTERS];
    uint16_t address_register;
    uint16_t program_counter;
    uint16_t stack[EMU_MAX_STACK_SIZE];
    uint8_t delay_register;
    uint8_t sound_register;
    uint8_t current_stack;
    uint8_t skip_fetch;
} emu_registers_state_t;

typedef struct s_emu_state
{
    emu_registers_state_t registers;
    int32_t keys_state[EMU_NB_KEYS];
    uint8_t framebuffer[EMU_FRAMEBUFFER_SIZE];
} emu_state_t;

#endif // CHIP8_EMU_EMU_STATE_H
