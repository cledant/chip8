#ifndef CHIP8_EMU_RENDERER_H
#define CHIP8_EMU_RENDERER_H

#include <stdint.h>

int renderer_init(char const **err);
void renderer_destroy();
int renderer_create_window(int32_t w, int32_t h, char const **err);
void renderer_destroy_window();
int renderer_create_framebuffer(int32_t fb_w, int32_t fb_h, char const **err);
void renderer_set_colors(int32_t background, int32_t sprite);
void renderer_invert_background_color();
int renderer_draw(void const *one_bit_depth_fb);

#endif // CHIP8_EMU_RENDERER_H
