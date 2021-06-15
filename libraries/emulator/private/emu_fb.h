#ifndef CHIP8_EMU_EMU_FB_H
#define CHIP8_EMU_EMU_FB_H

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
 * Size
 */
#define EMU_CHIP8_FRAMEBUFFER_SIZE (EMU_CHIP8_W * EMU_CHIP8_H / 8)
#define EMU_FRAMEBUFFER_MAX_SIZE (EMU_SUPER_CHIP_8_W * EMU_SUPER_CHIP_8_H / 8)

#endif // CHIP8_EMU_EMU_FB_H
