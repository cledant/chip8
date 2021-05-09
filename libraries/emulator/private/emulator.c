#include "emulator.h"

#include <stdint.h>
#include <stdio.h>

#include "emu_loop.h"

/*
 * Public strings
 */
char const *emu_rom_types_str[EMU_RT_NB_TYPES] = { "NONE",
                                                   "CHIP8",
                                                   "CHIP8_HI_RES",
                                                   "SUPERCHIP8" };

/*
 * Structs
 */
typedef struct s_chip_8_registers_state
{
    uint8_t general_registers[EMU_NB_REGISTERS_MAX];
    uint16_t address_register;
    uint16_t program_counter;
    uint16_t stack[EMU_MAX_STACK_SIZE];
    uint8_t current_stack;
} chip_8_registers_state_t;

typedef struct s_emu_chip_8_state
{
    emu_rom_type_t rom_type;
    uint8_t ram[EMU_CHIP_8_ROM_SIZE];
    int32_t keys_state[EMU_KEY_NB];
    chip_8_registers_state_t registers;
    uint8_t framebuffer[EMU_FRAMEBUFFER_SIZE];
} emu_chip_8_state_t;

/*
 * Emulator variables
 */
static emu_chip_8_state_t emu_state;
static uint8_t const emu_chip_8_fonts[EMU_FONT_NB][EMU_CHIP_8_FONT_HEIGHT] = {
    { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, { 0x20, 0x60, 0x20, 0x20, 0x70 },
    { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x10, 0xF0, 0x10, 0xF0 },
    { 0x90, 0x90, 0xF0, 0x10, 0x10 }, { 0xF0, 0x80, 0xF0, 0x10, 0xF0 },
    { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x10, 0x20, 0x40, 0x40 },
    { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x90, 0xF0, 0x10, 0xF0 },
    { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, { 0xE0, 0x90, 0xE0, 0x90, 0xE0 },
    { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, { 0xE0, 0x90, 0x90, 0x90, 0xE0 },
    { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x80, 0xF0, 0x80, 0x80 }
};

/*
 * Functions
 */
int
emu_load_rom(char const *rom_path, emu_rom_type_t rom_type, char const **err)
{
    (void)emu_state;
    (void)rom_path;
    (void)rom_type;
    (void)emu_chip_8_fonts;
    *err = "TODO ROM LOADING";
    return (1);
}

int
emu_start(char const **err)
{
    if (emu_state.rom_type == EMU_RT_NONE) {
        *err = "No Rom loaded";
        return (1);
    }
    return (emu_loop(err));
}
