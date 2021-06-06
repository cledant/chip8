#ifndef CHIP8_EMU_EMU_DEF_H
#define CHIP8_EMU_EMU_DEF_H

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
 * Ram Locations
 */
#define EMU_RAM_ENTRY_POINT 0x200
#define EMU_CHIP8_MAX_PROG_RAM_ADDR (0xE90 - 1)
#define EMU_SUPER_CHIP8_MAX_PROG_RAM_ADDR (0xFFF - 1)

/*
 * Sizes
 */
#define EMU_CHIP_8_ROM_SIZE (0xFFF + 1)
#define EMU_CHIP8_MAX_ROM_SIZE (EMU_CHIP_8_ROM_SIZE - EMU_RAM_ENTRY_POINT)
#define EMU_MAX_STACK_SIZE 16
#define EMU_CHIP_8_FONT_HEIGHT 5
#define EMU_FRAMEBUFFER_SIZE (EMU_SUPER_CHIP_8_W * EMU_SUPER_CHIP_8_H / 8)
#define EMU_CHIP8_NB_INST 33

/*
 * Other
 */
#define EMU_KEY_PRESSED 1
#define EMU_KEY_RELEASED 0

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

typedef enum e_emu_keys
{
    EMU_KEY_0 = 0,
    EMU_KEY_1,
    EMU_KEY_2,
    EMU_KEY_3,
    EMU_KEY_4,
    EMU_KEY_5,
    EMU_KEY_6,
    EMU_KEY_7,
    EMU_KEY_8,
    EMU_KEY_9,
    EMU_KEY_A,
    EMU_KEY_B,
    EMU_KEY_C,
    EMU_KEY_D,
    EMU_KEY_E,
    EMU_KEY_F,
    EMU_KEY_NB,
} emu_keys_t;

#endif // CHIP8_EMU_EMU_DEF_H
