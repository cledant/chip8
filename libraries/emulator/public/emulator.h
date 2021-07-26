#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include <stdint.h>

typedef enum e_emu_rom_type
{
    EMU_RT_NONE = 0,
    EMU_RT_CHIP_8_MODERN,
    EMU_RT_SUPER_CHIP_8,
    EMU_RT_CHIP_8_COSMAC,
    EMU_RT_NB_TYPES,
} emu_rom_type_t;
extern char const *g_emu_rom_types_str[EMU_RT_NB_TYPES];

int emu_load_rom(char const *rom_path,
                 emu_rom_type_t rom_type,
                 char const **err);
int emu_get_framebuffer_size(int32_t *w, int32_t *h);
int emu_press_key(int32_t key_value);
int emu_release_key(int32_t key_value);
void emu_decrement_timers();
int emu_fetch(char const **err);
int emu_decode(char const **err);
int emu_execute(char const **err);
void const *emu_get_framebuffer();
int emu_is_sound_active();
int emu_should_exit();

#endif // CHIP8_EMULATOR_H
