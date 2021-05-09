#ifndef CHIP8_EMU_EMU_LOOP_H
#define CHIP8_EMU_EMU_LOOP_H

#include "emu_def.h"

int emu_fetch(char const **err);
int emu_decode(char const **err);
int emu_execute(char const **err);
int emu_loop(char const **err);

#endif // CHIP8_EMU_EMU_LOOP_H
