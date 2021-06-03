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
    int32_t pixel_rgba;
    int32_t fb_w;
    int32_t fb_h;
} t_renderer;

/*
 * Renderer variables
 */
static t_renderer rdr;

/*
 * Internal functions
 */
static void
copy_emulator_fb(void const *one_bit_depth_fb)
{
    void *ptr_tex;
    int32_t useless;

    SDL_LockTexture(rdr.tex, NULL, &ptr_tex, &useless);
    int32_t total_size = rdr.fb_h * rdr.fb_w;
    uint8_t const *emu_buff = one_bit_depth_fb;
    int32_t *tex_buff = ptr_tex;
    uint8_t mask = 0;
    int32_t j = 0;

    for (int32_t i = 0; i < total_size; ++i) {
        int32_t bit_value = ((1 << mask) & emu_buff[j]) >> mask;
        tex_buff[i] = rdr.pixel_rgba * bit_value;
        mask = (mask + 1) % 8;
        if (!mask) {
            ++j;
        }
    }
    SDL_UnlockTexture(rdr.tex);
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
        return (-1);
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
    rdr.win =
      SDL_CreateWindow("chip8_emu",
                       SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED,
                       w,
                       h,
                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rdr.win) {
        *err = "Failed to create window";
        return (1);
    }
    rdr.renderer = SDL_CreateRenderer(rdr.win, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!rdr.renderer) {
        *err = "Failed to create renderer";
        return (1);
    }
    return (0);
}

void
renderer_destroy_window()
{
    if (rdr.tex) {
        SDL_DestroyTexture(rdr.tex);
    }
    if (rdr.renderer) {
        SDL_DestroyRenderer(rdr.renderer);
    }
    if (rdr.win) {
        SDL_DestroyWindow(rdr.win);
    }
}

int
renderer_create_framebuffer(int32_t fb_w,
                            int32_t fb_h,
                            int32_t pixel_rgba,
                            char const **err)
{
    rdr.tex = SDL_CreateTexture(rdr.renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                fb_w,
                                fb_h);
    if (!rdr.tex) {
        *err = "Failed to create renderer texture";
        return (1);
    }
    rdr.pixel_rgba = pixel_rgba;
    rdr.fb_w = fb_w;
    rdr.fb_h = fb_h;
    return (0);
}

int
renderer_draw(void const *one_bit_depth_fb)
{
    SDL_SetRenderTarget(rdr.renderer, rdr.tex);
    SDL_RenderClear(rdr.renderer);
    copy_emulator_fb(one_bit_depth_fb);
    SDL_RenderCopy(rdr.renderer, rdr.tex, NULL, NULL);
    SDL_RenderPresent(rdr.renderer);
    return (0);
}
