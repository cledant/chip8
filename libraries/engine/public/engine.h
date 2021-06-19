#ifndef CHIP8_EMU_ENGINE_H
#define CHIP8_EMU_ENGINE_H

#include <stdint.h>

int engine_init(uint32_t cycles_per_frame);
int engine_loop();

#endif // CHIP8_EMU_ENGINE_H
