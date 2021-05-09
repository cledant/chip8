#include "renderer.h"

#include <stdio.h>

#include "SDL.h"

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
renderer_create_window(int32_t w, int32_t h, char const *err)
{
    (void)err;
    (void)w;
    (void)h;
    puts("TODO renderer_create_window");
    return (0);
}
