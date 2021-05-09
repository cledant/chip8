#ifndef CHIP8_EMU_RENDERER_H
#define CHIP8_EMU_RENDERER_H

#include <stdint.h>

int renderer_init(char const **err);
void renderer_destroy();
int renderer_create_window(int32_t w, int32_t h, char const *err);
void renderer_destroy_window();

#endif // CHIP8_EMU_RENDERER_H
