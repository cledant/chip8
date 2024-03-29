#include "emulator.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "emu_state.h"
#include "emu_inst.h"
#include "emu_chip8.h"
#include "emu_chip8_cosmac_vip.h"
#include "emu_superchip8.h"
#include "emu_quirks.h"

#define EMU_KEY_PRESSED 1
#define EMU_KEY_RELEASED 0
#define EMU_ERR_BUFFER_SIZE 1024

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/*
 * Emulator variables
 */
static emu_rom_type_t emu_rom_type;
static emu_state_t emu_state;
static emu_parse_fct_t *emu_parse_fcts;
static uint32_t emu_nb_inst;
static uint64_t emu_options;
static emu_inst_t emu_curr_inst;
static emu_exec_fct_t emu_exec_fct;
static char emu_err_buffer[EMU_ERR_BUFFER_SIZE];

/*
 * Emulator constants
 */
static uint8_t const
  emu_chip_8_fonts[EMU_CHIP8_NB_FONTS][EMU_CHIP_8_FONT_HEIGHT] = {
      { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, { 0x20, 0x60, 0x20, 0x20, 0x70 },
      { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x10, 0xF0, 0x10, 0xF0 },
      { 0x90, 0x90, 0xF0, 0x10, 0x10 }, { 0xF0, 0x80, 0xF0, 0x10, 0xF0 },
      { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x10, 0x20, 0x40, 0x40 },
      { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, { 0xF0, 0x90, 0xF0, 0x10, 0xF0 },
      { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, { 0xE0, 0x90, 0xE0, 0x90, 0xE0 },
      { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, { 0xE0, 0x90, 0x90, 0x90, 0xE0 },
      { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, { 0xF0, 0x80, 0xF0, 0x80, 0x80 }
  };

static uint8_t const emu_super_chip_8_fonts
  [EMU_SUPER_CHIP_8_FONT][EMU_SUPER_CHIP_8_FONT_HEIGHT] = {
      { 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF },
      { 0x18, 0x18, 0x78, 0x78, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF },
      { 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF },
      { 0xFF, 0xFF, 0x03, 0x03, 0x3F, 0x3F, 0x03, 0x03, 0xFF, 0xFF },
      { 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03 },
      { 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF },
      { 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF },
      { 0xFF, 0xFF, 0x03, 0x03, 0x07, 0x0E, 0x1C, 0x18, 0x18, 0x18 },
      { 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF },
      { 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF },
  };

/*
 * CHIP8 KEYBOARD LAYOUT
 * 1 | 2 | 3 | C
 * 4 | 5 | 6 | D
 * 7 | 8 | 9 | E
 * A | 0 | B | F
 */
static int const emu_key_values[EMU_NB_KEYS] = { 'X', '1', '2', '3', 'Q', 'W',
                                                 'E', 'A', 'S', 'D', 'Z', 'C',
                                                 '4', 'R', 'F', 'V' };

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
        if (err) {
            *err = "Failed to open rom";
        }
        return (1);
    }
    if (fseek(*rom_file, 0, SEEK_END)) {
        if (err) {
            *err = "Fseek failed";
        }
        return (1);
    }
    *rom_size = ftell(*rom_file);
    if (*rom_size > EMU_MAX_ROM_SIZE || *rom_size < 0) {
        fclose(*rom_file);
        if (err) {
            *err = "Rom file too big";
        }
        return (1);
    }
    if (fseek(*rom_file, 0, SEEK_SET)) {
        fclose(*rom_file);
        if (err) {
            *err = "Fseek failed";
        }
        return (1);
    }
    return (0);
}

static int
setup_specific_rom_type(uint64_t quirks, char const **err)
{
    switch (emu_rom_type) {
        case EMU_RT_CHIP_8_MODERN:
            emu_state.max_addr = EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR;
            emu_state.max_fb = EMU_CHIP8_FRAMEBUFFER_SIZE;
            emu_nb_inst = EMU_CHIP8_NB_INST;
            emu_parse_fcts = g_chip8_parse_fcts;
            g_chip8_parse_fcts[EMU_CHIP8_DRAW_INST] =
              (IS_EMU_QUIRK_DRAW_WRAP(quirks)) ? chip8_is_draw_wrap
                                               : chip8_is_draw;
            g_chip8_parse_fcts[EMU_CHIP8_JP_VO_ADDR] =
              (IS_EMU_QUIRK_BXNN_INST(quirks)) ? chip8_is_jp_v0_addr_quirk
                                               : chip8_is_jp_v0_addr;
            return (0);
        case EMU_RT_SUPER_CHIP_8:
            emu_state.max_addr = EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR;
            emu_state.max_fb = EMU_SUPER_CHIP8_FRAMEBUFFER_SIZE;
            emu_nb_inst = EMU_SUPER_CHIP8_NB_INST;
            emu_parse_fcts = g_superchip8_parse_fcts;
            g_superchip8_parse_fcts[EMU_SUPERCHIP8_DRAW_INST] =
              (IS_EMU_QUIRK_DRAW_WRAP(quirks)) ? superchip8_is_draw_wrap
                                               : superchip8_is_draw;
            g_superchip8_parse_fcts[EMU_SUPERCHIP8_DRAW_EXTENDED_INST] =
              (IS_EMU_QUIRK_DRAW_WRAP(quirks))
                ? superchip8_is_draw_extended_wrap
                : superchip8_is_draw_extended;
            g_superchip8_parse_fcts[EMU_SUPERCHIP8_JP_VO_ADDR] =
              (IS_EMU_QUIRK_BXNN_INST(quirks)) ? chip8_is_jp_v0_addr
                                               : chip8_is_jp_v0_addr_quirk;
            return (0);
        case EMU_RT_CHIP_8_COSMAC_VIP:
            emu_state.max_addr = EMU_CHIP8_COSMAC_MAX_PROG_RAM_ADDR;
            emu_state.max_fb = EMU_CHIP8_FRAMEBUFFER_SIZE;
            emu_nb_inst = EMU_CHIP8_NB_INST;
            emu_parse_fcts = g_chip8_cosmac_vip_parse_fcts;
            g_chip8_cosmac_vip_parse_fcts[EMU_CHIP8_DRAW_INST] =
              (IS_EMU_QUIRK_DRAW_WRAP(quirks)) ? chip8_is_draw_wrap
                                               : chip8_is_draw;
            g_chip8_cosmac_vip_parse_fcts[EMU_CHIP8_JP_VO_ADDR] =
              (IS_EMU_QUIRK_BXNN_INST(quirks)) ? chip8_is_jp_v0_addr_quirk
                                               : chip8_is_jp_v0_addr;
            return (0);
        case EMU_RT_NONE:
        default:
            if (err) {
                *err = "Loading invalid rom type";
            }
            emu_rom_type = EMU_RT_NONE;
            return (1);
    }
}

/*
 * Public Api
 */
char const *g_emu_rom_types_str[EMU_RT_NB_TYPES] = { "NONE",
                                                     "CHIP8",
                                                     "SUPERCHIP8",
                                                     "CHIP8_COSMAC_VIP" };

int
emu_load_rom(char const *rom_path,
             emu_rom_type_t rom_type,
             uint64_t quirks,
             uint64_t options,
             char const **err)
{
    FILE *rom_file = NULL;
    long rom_size = 0;

    if (!rom_path) {
        if (err) {
            *err = "Empty rom path";
        }
        return (1);
    }
    if (open_rom_file(rom_path, &rom_size, &rom_file, err)) {
        return (1);
    }
    memset(&emu_state, 0, sizeof(emu_state_t));
    memcpy(emu_state.ram, emu_chip_8_fonts, sizeof(emu_chip_8_fonts));
    memcpy(emu_state.ram + sizeof(emu_chip_8_fonts),
           emu_super_chip_8_fonts,
           sizeof(emu_super_chip_8_fonts));
    emu_state.current_mode_w = EMU_CHIP8_W;
    emu_state.current_mode_h = EMU_CHIP8_H;
    if (fread(
          emu_state.ram + EMU_CHIP8_RAM_ENTRY_POINT, 1, rom_size, rom_file) !=
        (size_t)rom_size) {
        fclose(rom_file);
        if (err) {
            *err = "Failed to read Rom";
        }
        return (1);
    }
    fclose(rom_file);

    emu_state.registers.program_counter = EMU_CHIP8_RAM_ENTRY_POINT;
    emu_rom_type = rom_type;
    if (setup_specific_rom_type(quirks, err)) {
        return (1);
    }
    srand48(time(NULL));
    emu_options = options;
    return (0);
}

int
emu_get_low_res_size(int32_t *w, int32_t *h)
{
    if (emu_rom_type == EMU_RT_CHIP_8_MODERN ||
        emu_rom_type == EMU_RT_CHIP_8_COSMAC_VIP ||
        emu_rom_type == EMU_RT_SUPER_CHIP_8) {
        *w = EMU_CHIP8_W;
        *h = EMU_CHIP8_H;
    } else {
        *w = 0;
        *h = 0;
        return (1);
    }
    return (0);
}

int
emu_get_high_res_size(int32_t *w, int32_t *h)
{
    if (emu_rom_type == EMU_RT_CHIP_8_MODERN ||
        emu_rom_type == EMU_RT_CHIP_8_COSMAC_VIP) {
        *w = EMU_CHIP8_W;
        *h = EMU_CHIP8_H;
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
emu_get_current_resolution_mode(int32_t *mode)
{
    if (!mode) {
        return (1);
    }
    *mode = emu_state.res_mode;
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

void
emu_decrement_timers()
{
    if (emu_state.registers.delay_register) {
        --emu_state.registers.delay_register;
    }
    if (emu_state.registers.sound_register)
        --emu_state.registers.sound_register;
}

int
emu_fetch(char const **err)
{
    if (emu_state.skip_fetch) {
        return (0);
    }
    if (emu_state.registers.program_counter % 2 &&
        IS_EMU_OPTION_WARN_NOT_ALIGNED(emu_options)) {
        printf("[WARN][FETCH]: PC (%x) not even aligned\n",
               emu_state.registers.program_counter);
    }
    if (emu_state.registers.program_counter > emu_state.max_addr) {
        snprintf(
          emu_err_buffer,
          EMU_ERR_BUFFER_SIZE,
          "Program counter is outside allowed range=0x%x. Max allowed=0x%x",
          emu_state.registers.program_counter,
          emu_state.max_addr);
        if (err) {
            *err = emu_err_buffer;
        }
        emu_curr_inst = (emu_inst_t){ .n1 = 0, .n2 = 0, .n3 = 0, .n4 = 0 };
        return (1);
    }

    uint16_t *ram_data =
      (uint16_t *)&emu_state.ram[emu_state.registers.program_counter];
    emu_curr_inst.n1 = ((*ram_data & 0x00F0) >> 4);
    emu_curr_inst.n2 = ((*ram_data & 0x000F));
    emu_curr_inst.n3 = ((*ram_data & 0xF000) >> 12);
    emu_curr_inst.n4 = ((*ram_data & 0x0F00) >> 8);
    emu_state.registers.program_counter += 2;
    return (0);
}

int
emu_decode(char const **err)
{
    for (uint32_t i = 0; i < emu_nb_inst; ++i) {
        if ((emu_exec_fct = ((emu_parse_fcts)[i])(emu_curr_inst))) {
            return (0);
        }
    }
    snprintf(emu_err_buffer,
             EMU_ERR_BUFFER_SIZE,
             "Invalid OP code (0x%x%x%x%x) at PC = 0x%x",
             emu_curr_inst.n1,
             emu_curr_inst.n2,
             emu_curr_inst.n3,
             emu_curr_inst.n4,
             emu_state.registers.program_counter - 2);
    if (err) {
        *err = emu_err_buffer;
    }
    return (1);
}

int
emu_execute(char const **err)
{
    if (emu_exec_fct) {
        int ret = (emu_exec_fct)(emu_curr_inst, &emu_state, err);
        emu_exec_fct = NULL;
        return (ret);
    }
    return (0);
}

void const *
emu_get_framebuffer()
{
    return (emu_state.framebuffer);
}

int
emu_is_sound_active()
{
    return (emu_state.registers.sound_register);
}

int
emu_should_exit()
{
    return (emu_state.should_exit);
}

int
emu_open_flag_registers_file(char const *filepath, char const **err)
{
    if (!filepath) {
        if (err) {
            *err = "Empty filepath";
        }
        return (1);
    }

    FILE *file = fopen(filepath, "r");
    if (!file) {
        if (err) {
            *err = "Fopen failed";
        }
        return (1);
    }
    if (fseek(file, 0, SEEK_END)) {
        fclose(file);
        if (err) {
            *err = "Fseek failed";
        }
        return (1);
    }
    long flag_registers_size = ARRAY_SIZE(emu_state.registers.flag_registers);
    long file_size = ftell(file);
    if (file_size > flag_registers_size || file_size < 0) {
        fclose(file);
        if (err) {
            *err = "Flag registers file too big";
        }
        return (1);
    }
    if (fseek(file, 0, SEEK_SET)) {
        fclose(file);
        if (err) {
            *err = "Fseek failed";
        }
        return (1);
    }
    if (fread(
          emu_state.registers.flag_registers, 1, flag_registers_size, file) !=
        (size_t)flag_registers_size) {
        fclose(file);
        if (err) {
            *err = "Failed to read Rom";
        }
        return (1);
    }
    fclose(file);
    return (0);
}

int
emu_save_flag_registers_to_file(char const *filepath, char const **err)
{
    if (!filepath) {
        if (err) {
            *err = "Empty filepath";
        }
        return (1);
    }

    FILE *file = fopen(filepath, "w");
    if (!file) {
        if (err) {
            *err = "Fopen failed";
        }
        return (1);
    }
    fwrite(emu_state.registers.flag_registers,
           ARRAY_SIZE(emu_state.registers.flag_registers),
           1,
           file);
    fclose(file);
    return 0;
}
