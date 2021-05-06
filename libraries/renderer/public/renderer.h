#ifndef CHIP8_EMU_RENDERER_H
#define CHIP8_EMU_RENDERER_H

int renderer_init(char const **err);
void renderer_destroy();
int renderer_create_window(char const *err);

#endif // CHIP8_EMU_RENDERER_H
