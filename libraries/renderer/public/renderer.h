#ifndef CHIP8_EMU_RENDERER_H
#define CHIP8_EMU_RENDERER_H

#include <stdint.h>

int renderer_init(char const **err);
void renderer_destroy();
int renderer_create_window(int32_t w,
                           int32_t h,
                           int32_t border_px,
                           char const **err);
void renderer_destroy_window();
int renderer_create_framebuffer(int32_t fb_w, int32_t fb_h, char const **err);
void renderer_set_colors(int32_t background,
                         int32_t sprite,
                         int32_t silent,
                         int32_t sound);
int renderer_draw(void const *one_bit_depth_fb, int is_sound_active);

#endif // CHIP8_EMU_RENDERER_H
