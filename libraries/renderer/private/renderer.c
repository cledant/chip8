#include "renderer.h"

#include "SDL.h"

#include <string.h>

/*
 * Structs
 */
typedef struct s_renderer
{
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Texture *tex;
    int32_t color_mod_rgb;
    int32_t fb_w;
    int32_t fb_h;
} renderer_t;

/*
 * Renderer variables
 */
static renderer_t rdr_env;

/*
 * Internal functions
 */
static void
copy_emulator_fb(void const *one_bit_depth_fb)
{
    void *ptr_tex;
    int32_t useless;
    SDL_LockTexture(rdr_env.tex, NULL, &ptr_tex, &useless);
    int32_t *tex_buff = ptr_tex;
    uint8_t const *emu_buff = one_bit_depth_fb;

    uint8_t mask = 0;
    int32_t j = 0;
    int32_t total_size = rdr_env.fb_h * rdr_env.fb_w;
    for (int32_t i = 0; i < total_size; ++i) {
        int32_t bit_value = ((1 << mask) & emu_buff[j]) >> mask;
        tex_buff[i] = rdr_env.color_mod_rgb * bit_value;
        mask = (mask + 1) % 8;
        if (!mask) {
            ++j;
        }
    }
    SDL_UnlockTexture(rdr_env.tex);
}

/*
 * Public API
 */
int
renderer_init(char const **err)
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        if (err) {
            *err = "Failed to Init SDL2 Video";
        }
        return (1);
    }
    atexit(SDL_Quit);
    return (0);
}

void
renderer_destroy()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int
renderer_create_window(int32_t w, int32_t h, char const **err)
{
    rdr_env.win =
      SDL_CreateWindow("chip8_emu",
                       SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED,
                       w,
                       h,
                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rdr_env.win) {
        *err = "Failed to create window";
        return (1);
    }
    rdr_env.renderer =
      SDL_CreateRenderer(rdr_env.win, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!rdr_env.renderer) {
        *err = "Failed to create renderer";
        return (1);
    }
    return (0);
}

void
renderer_destroy_window()
{
    if (rdr_env.tex) {
        SDL_DestroyTexture(rdr_env.tex);
    }
    if (rdr_env.renderer) {
        SDL_DestroyRenderer(rdr_env.renderer);
    }
    if (rdr_env.win) {
        SDL_DestroyWindow(rdr_env.win);
    }
}

int
renderer_create_framebuffer(int32_t fb_w,
                            int32_t fb_h,
                            int32_t color_mod_rgb,
                            char const **err)
{
    rdr_env.tex = SDL_CreateTexture(rdr_env.renderer,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    fb_w,
                                    fb_h);
    if (!rdr_env.tex) {
        *err = "Failed to create renderer texture";
        return (1);
    }
    rdr_env.color_mod_rgb = color_mod_rgb;
    rdr_env.fb_w = fb_w;
    rdr_env.fb_h = fb_h;
    return (0);
}

int
renderer_draw(void const *one_bit_depth_fb)
{
    SDL_SetRenderTarget(rdr_env.renderer, rdr_env.tex);
    SDL_RenderClear(rdr_env.renderer);
    copy_emulator_fb(one_bit_depth_fb);
    SDL_RenderCopy(rdr_env.renderer, rdr_env.tex, NULL, NULL);
    SDL_RenderPresent(rdr_env.renderer);
    return (0);
}
