#ifndef CHIP8_EMU_ENGINE_H
#define CHIP8_EMU_ENGINE_H

#include <stdint.h>

#define ENGINE_DEFAULT_CYCLES_PER_FRAME 30

/*
 * Options
 */
#define ENGINE_OPTION_MUTE_SOUND 1UL
#define IS_ENGINE_OPTION_MUTE_SOUND(field) ((field)&ENGINE_OPTION_MUTE_SOUND)

int engine_init(uint32_t cycles_per_frame, uint64_t options);
int engine_loop();

#endif // CHIP8_EMU_ENGINE_H
