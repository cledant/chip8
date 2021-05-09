#include "emulator.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
static int
emu_open_rom_file(char const *rom_path,
                  long *rom_size,
                  FILE **rom_file,
                  char const **err)
{
    *rom_file = fopen(rom_path, "r");
    if (!*rom_file) {
        *err = "Failed to open rom";
        return (1);
    }
    if (fseek(*rom_file, 0, SEEK_END)) {
        *err = "fseek failed";
        return (1);
    }
    *rom_size = ftell(*rom_file);
    if (*rom_size > EMU_CHIP8_MAX_ROM_SIZE || *rom_size < 0) {
        fclose(*rom_file);
        *err = "Rom file too big";
        return (1);
    }
    if (fseek(*rom_file, 0, SEEK_SET)) {
        fclose(*rom_file);
        *err = "fseek failed";
        return (1);
    }
    return (0);
}

int
emu_get_framebuffer_size(int32_t *w, int32_t *h)
{
    if (emu_state.rom_type == EMU_RT_CHIP_8) {
        *w = EMU_CHIP8_W;
        *h = EMU_CHIP8_H;
    } else if (emu_state.rom_type == EMU_RT_CHIP_8) {
        *w = EMU_CHIP8_HI_RES_W;
        *h = EMU_CHIP8_HI_RES_H;
    } else if (emu_state.rom_type == EMU_RT_SUPER_CHIP_8) {
        *w = EMU_SUPER_CHIP_8_W;
        *h = EMU_SUPER_CHIP_8_W;
    } else {
        *w = 0;
        *h = 0;
        return (1);
    }
    return (0);
}

int
emu_load_rom(char const *rom_path, emu_rom_type_t rom_type, char const **err)
{
    FILE *rom_file = NULL;
    long rom_size = 0;

    if (emu_open_rom_file(rom_path, &rom_size, &rom_file, err)) {
        return (1);
    }
    memset(&emu_state, 0, sizeof(emu_chip_8_state_t));
    if (fread(emu_state.ram + EMU_RAM_ENTRY_POINT, rom_size, 1, rom_file) !=
        1) {
        fclose(rom_file);
        *err = "Failed to read Rom";
        return (1);
    }
    memcpy(emu_state.ram,
           emu_chip_8_fonts,
           sizeof(uint8_t) * EMU_FONT_NB * EMU_CHIP_8_FONT_HEIGHT);
    emu_state.registers.program_counter = EMU_RAM_ENTRY_POINT;
    emu_state.rom_type = rom_type;
    return (0);
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
