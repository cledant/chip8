#ifndef CHIP8_EMU_INPUT_H
#define CHIP8_EMU_INPUT_H

#include <stdint.h>

int input_init(char const **err);
void input_destroy();
int input_get(char const **err);

#endif // CHIP8_EMU_INPUT_H
