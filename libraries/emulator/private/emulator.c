#include "emulator.h"

#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
 * Framebuffers resolutions
 */
#define EMU_CHIP8_W 64
#define EMU_CHIP8_H 32
#define EMU_CHIP8_HI_RES_W 64
#define EMU_CHIP8_HI_RES_H 64
#define EMU_SUPER_CHIP_8_W 128
#define EMU_SUPER_CHIP_8_H 64

/*
 * Sizes
 */
#define EMU_CHIP_8_ROM_SIZE 0xFFF
#define EMU_MAX_STACK_SIZE 16
#define EMU_CHIP_8_FONT_HEIGHT 5
#define EMU_FRAMEBUFFER_SIZE (EMU_SUPER_CHIP_8_W * EMU_SUPER_CHIP_8_H / 8)

/*
 * Ram Locations
 */
#define EMU_RAM_ENTRY_POINT 0x200
#define EMU_CHIP8_MAX_PROG_RAM_ADDR 0xE90
#define EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR 0xFFE

/*
 * Enums
 */
typedef enum e_emu_chip_8_registers
{
    EMU_V0 = 0,
    EMU_V1,
    EMU_V2,
    EMU_V3,
    EMU_V4,
    EMU_V5,
    EMU_V6,
    EMU_V7,
    EMU_V8,
    EMU_V9,
    EMU_VA,
    EMU_VB,
    EMU_VC,
    EMU_VD,
    EMU_VE,
    EMU_VF,
    EMU_DELAY_TIMER,
    EMU_SOUND_TIMER,
    EMU_NB_REGISTERS_MAX,
} emu_chip_8_registers;

typedef enum e_emu_fonts
{
    EMU_FONT_0 = 0,
    EMU_FONT_1,
    EMU_FONT_2,
    EMU_FONT_3,
    EMU_FONT_4,
    EMU_FONT_5,
    EMU_FONT_6,
    EMU_FONT_7,
    EMU_FONT_8,
    EMU_FONT_9,
    EMU_FONT_A,
    EMU_FONT_B,
    EMU_FONT_C,
    EMU_FONT_D,
    EMU_FONT_E,
    EMU_FONT_F,
    EMU_FONT_NB,
} emu_fonts_t;

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
 * Debug
 */
#define EMU_DEBUG_STR_SIZE 2048
static char emu_debug_register_states_array[EMU_DEBUG_STR_SIZE];

/*
 * Functions
 */
int
emu_load_rom(char const *rom_path, emu_rom_type_t rom_type, char const **err)
{
    (void)rom_path;
    (void)rom_type;
    (void)err;
    (void)emu_chip_8_fonts;
    return (0);
}

void *
emu_get_framebuffer(int32_t *w, int32_t *h)
{
    assert(w && h);
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
        return (NULL);
    }
    return (emu_state.framebuffer);
}

void
emu_set_key_state(int32_t const *key_state)
{
    assert(key_state);
    memcpy(emu_state.keys_state, key_state, sizeof(int32_t) * EMU_KEY_NB);
}

int
emu_fetch(char const **err)
{
    (void)err;
    return (0);
}

int
emu_decode(char const **err)
{
    (void)err;
    return (0);
}

int
emu_execute(char const **err)
{
    (void)err;
    return (0);
}

char const *
emu_get_debug_register_states()
{
    return (emu_debug_register_states_array);
}
