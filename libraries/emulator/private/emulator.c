#include "emulator.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "emu_state.h"
#include "emu_inst.h"
#include "emu_def.h"
#include "emu_chip8.h"

/*
 * Emulator variables
 */
static emu_rom_type_t emu_rom_type;
static uint8_t emu_ram[EMU_CHIP_8_RAM_SIZE];
static emu_state_t emu_state;
static draw_fct_t emu_draw_fct;
static uint32_t emu_max_addr;
static emu_parse_fct_t *emu_parse_fcts;
static uint32_t emu_nb_inst;
static uint16_t *emu_inst_data;
static emu_exec_fct_t emu_exec_fct;

/*
 * Emulator constants
 */
static uint8_t const emu_chip_8_fonts[EMU_NB_FONTS][EMU_CHIP_8_FONT_HEIGHT] = {
    { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, { 0x20, 0x60, 0x20, 0x20, 0x70 },
    { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x10, 0xF0, 0x10, 0xF0 },
    { 0x90, 0x90, 0xF0, 0x10, 0x10 }, { 0xF0, 0x80, 0xF0, 0x10, 0xF0 },
    { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x10, 0x20, 0x40, 0x40 },
    { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x90, 0xF0, 0x10, 0xF0 },
    { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, { 0xE0, 0x90, 0xE0, 0x90, 0xE0 },
    { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, { 0xE0, 0x90, 0x90, 0x90, 0xE0 },
    { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x80, 0xF0, 0x80, 0x80 }
};
static int const emu_key_values[EMU_NB_KEYS] = {
    '1', '2', '3', '4', 'Q', 'W', 'E', 'R',
    'A', 'S', 'D', 'F', 'Z', 'X', 'C', 'V',
};

/*
 * Internal functions
 */
static int
open_rom_file(char const *rom_path,
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

/*
 * Public Api
 */
char const *g_emu_rom_types_str[EMU_RT_NB_TYPES] = { "NONE",
                                                     "CHIP8",
                                                     "CHIP8_HI_RES",
                                                     "SUPERCHIP8" };

int
emu_load_rom(char const *rom_path, emu_rom_type_t rom_type, char const **err)
{
    FILE *rom_file = NULL;
    long rom_size = 0;

    if (open_rom_file(rom_path, &rom_size, &rom_file, err)) {
        return (1);
    }
    memset(emu_ram, 0, sizeof(uint8_t) * EMU_CHIP_8_RAM_SIZE);
    memcpy(emu_ram,
           emu_chip_8_fonts,
           sizeof(uint8_t) * EMU_NB_FONTS * EMU_CHIP_8_FONT_HEIGHT);
    if (fread(emu_ram + EMU_RAM_ENTRY_POINT, rom_size, 1, rom_file) != 1) {
        fclose(rom_file);
        *err = "Failed to read Rom";
        return (1);
    }
    fclose(rom_file);

    memset(&emu_state, 0, sizeof(emu_state_t));
    emu_state.registers.program_counter = EMU_RAM_ENTRY_POINT;
    emu_rom_type = rom_type;
    if (rom_type == EMU_RT_CHIP_8) {
        emu_max_addr = EMU_CHIP8_MAX_PROG_RAM_ADDR;
        emu_nb_inst = EMU_CHIP8_NB_INST;
        emu_parse_fcts = g_chip8_parse_fcts;
    } else if (rom_type == EMU_RT_CHIP_8_HI_RES) {
        emu_max_addr = EMU_CHIP8_MAX_PROG_RAM_ADDR;
        emu_nb_inst = EMU_CHIP8_HI_RES_NB_INST;
        emu_parse_fcts = NULL;
        *err = "Chip8 Hi Res not implemented yet";
        return (1);
    } else if (rom_type == EMU_RT_SUPER_CHIP_8) {
        emu_max_addr = EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR;
        emu_nb_inst = EMU_SUPER_CHIP8_NB_INST;
        emu_parse_fcts = NULL;
        *err = "Super Chip8 not implemented yet";
        return (1);
    } else {
        *err = "Loading invalid rom type";
        emu_rom_type = EMU_RT_NONE;
        return (1);
    }
    return (0);
}

void
emu_set_draw_fct(draw_fct_t ptr)
{
    emu_draw_fct = ptr;
}

int
emu_get_framebuffer_size(int32_t *w, int32_t *h)
{
    if (emu_rom_type == EMU_RT_CHIP_8) {
        *w = EMU_CHIP8_W;
        *h = EMU_CHIP8_H;
    } else if (emu_rom_type == EMU_RT_CHIP_8_HI_RES) {
        *w = EMU_CHIP8_HI_RES_W;
        *h = EMU_CHIP8_HI_RES_H;
    } else if (emu_rom_type == EMU_RT_SUPER_CHIP_8) {
        *w = EMU_SUPER_CHIP_8_W;
        *h = EMU_SUPER_CHIP_8_H;
    } else {
        *w = 0;
        *h = 0;
        return (1);
    }
    return (0);
}

int
emu_press_key(int key_value)
{
    for (int i = 0; i < EMU_NB_KEYS; ++i) {
        if (key_value == emu_key_values[i]) {
            emu_state.keys_state[i] = EMU_KEY_PRESSED;
            return (0);
        }
    }
    return (1);
}

int
emu_release_key(int key_value)
{
    for (int i = 0; i < EMU_NB_KEYS; ++i) {
        if (key_value == emu_key_values[i]) {
            emu_state.keys_state[i] = EMU_KEY_RELEASED;
            return (0);
        }
    }
    return (1);
}

int
emu_fetch(char const **err)
{
    static char err_buffer[256];

    if (emu_state.registers.program_counter % 2) {
        printf("[WARN][FETCH]: PC (%x) not even aligned\n",
               emu_state.registers.program_counter);
    }
    if (emu_state.registers.program_counter >= emu_max_addr) {
        snprintf(err_buffer,
                 256,
                 "Program counter is outside allowed range=%x. Max allowed=%x",
                 emu_state.registers.program_counter,
                 emu_max_addr);
        *err = err_buffer;
        emu_inst_data = NULL;
        return (1);
    }
    emu_inst_data = (uint16_t *)&emu_ram[emu_state.registers.program_counter];
    emu_state.registers.program_counter += 2;
    return (0);
}

int
emu_decode(char const **err)
{
    static char err_buffer[256];
    emu_inst_t inst = { .raw_data = *emu_inst_data };

    for (uint32_t i = 0; i < emu_nb_inst; ++i) {
        if ((emu_exec_fct = ((g_chip8_parse_fcts)[i])(inst))) {
            return (0);
        }
    }
    snprintf(err_buffer,
             256,
             "Invalid OP code at PC = %x",
             emu_state.registers.program_counter);
    *err = err_buffer;
    return (1);
}

int
emu_execute(char const **err)
{
    if (emu_exec_fct) {
        emu_inst_t inst = { .raw_data = *emu_inst_data };

        int ret = (emu_exec_fct)(inst, &emu_state, err);
        emu_exec_fct = NULL;
        return (ret);
    }
    return (0);
}
