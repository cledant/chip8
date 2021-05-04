#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum e_emu_rom_type
{
    EMU_RT_NONE = 0,
    EMU_RT_CHIP_8,
    EMU_RT_CHIP_8_HI_RES,
    EMU_RT_SUPER_CHIP_8,
    EMU_RT_NB_TYPES,
} emu_rom_type_t;

typedef enum e_emu_keys {
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

bool emu_load_rom(char const *rom_path,
                  emu_rom_type_t rom_type,
                  char const **err);
void *emu_get_framebuffer(int32_t *w, int32_t *h);
void emu_set_key_state(bool const key_state[EMU_KEY_NB]);
bool emu_fetch(char const **err);
bool emu_decode(char const **err);
bool emu_execute(char const **err);
char const *emu_get_debug_register_states();

#endif // CHIP8_EMULATOR_H
