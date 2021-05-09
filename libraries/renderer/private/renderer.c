#include "renderer.h"

#include "SDL.h"

/*
 * Defines
 */
#define WINDOW_INIT_POS_W 100
#define WINDOW_INIT_POS_H 100

/*
 * Structs
 */
typedef struct s_renderer
{
    SDL_Window *win;
    SDL_Renderer *renderer;
} t_renderer;

/*
 * Renderer variables
 */
static t_renderer sdl_renderer;

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
    sdl_renderer.win = SDL_CreateWindow(
      "chip8_emu", WINDOW_INIT_POS_W, WINDOW_INIT_POS_H, w, h, 0);
    if (!sdl_renderer.win) {
        *err = "Failed to create window";
        return (1);
    }
    sdl_renderer.renderer =
      SDL_CreateRenderer(sdl_renderer.win,
                         -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdl_renderer.renderer) {
        *err = "Failed to create renderer";
        return (1);
    }
    return (0);
}

void
renderer_destroy_window()
{
    SDL_DestroyRenderer(sdl_renderer.renderer);
    SDL_DestroyWindow(sdl_renderer.win);
}
