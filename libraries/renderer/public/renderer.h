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
int renderer_create_framebuffer(int32_t hires_w,
                                int32_t hires_h,
                                int32_t lores_w,
                                int32_t lores_h,
                                char const **err);
void renderer_set_colors(uint32_t background,
                         uint32_t sprite,
                         uint32_t silent,
                         uint32_t sound);
int renderer_draw(void const *one_bit_depth_fb,
                  int is_sound_active,
                  int res_mode);

#endif // CHIP8_EMU_RENDERER_H
