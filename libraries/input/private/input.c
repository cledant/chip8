#include "input.h"

#include "SDL.h"

#include "event_buffer.h"

/*
 * Key translation related
 */
#define NB_KEYS 16
static SDL_Scancode const input_key_scancode[NB_KEYS] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V,
};
static int32_t const input_key_value[NB_KEYS] = {
    '1', '2', '3', '4', 'Q', 'W', 'E', 'R',
    'A', 'S', 'D', 'F', 'Z', 'X', 'C', 'V',
};

static int
translate_scancode(SDL_Keysym *sdl_key_sym)
{
    for (int i = 0; i < NB_KEYS; ++i) {
        if (sdl_key_sym->scancode == input_key_scancode[i]) {
            return (input_key_value[i]);
        }
    }
    return (0);
}

/*
 * Event type handler
 */
static int
handle_close()
{
    event_t ev = { ET_SYS_CLOSE, 0, 0 };
    return (event_push(&ev));
}

static int
handle_win(SDL_Event *sdl_ev)
{
    event_t ev = { ET_NONE, 0, 0 };

    switch (sdl_ev->window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            ev.type = ET_SYS_CLOSE;
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            ev.type = ET_SYS_PAUSE;
            break;
        default:
            return (0);
    }
    return (event_push(&ev));
}

static int
handle_key(SDL_Event *sdl_ev)
{
    event_t ev = { ET_NONE, 0, 0 };

    if (sdl_ev->key.type == SDL_KEYDOWN &&
        sdl_ev->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        ev.type = ET_SYS_CLOSE;
        return (event_push(&ev));
    } else if (sdl_ev->key.type == SDL_KEYDOWN &&
               sdl_ev->key.keysym.scancode == SDL_SCANCODE_SPACE) {
        ev.type = ET_SYS_PAUSE;
        return (event_push(&ev));
    }
    switch (sdl_ev->key.type) {
        case SDL_KEYDOWN:
            ev.type = ET_KEY_PRESS;
            ev.data1 = translate_scancode(&sdl_ev->key.keysym);
            break;
        case SDL_KEYUP:
            ev.type = ET_KEY_RELEASE;
            ev.data1 = translate_scancode(&sdl_ev->key.keysym);
            break;
        default:
            return (0);
    }
    if (!ev.data1) {
        return (0);
    }
    return (event_push(&ev));
}

/*
 * Public Api
 */
int
input_init(char const **err)
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
input_destroy()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int
input_get(char const **err)
{
    (void)err;
    SDL_Event event;

    int ret = 0;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ret = handle_close();
                break;
            case SDL_WINDOWEVENT:
                ret = handle_win(&event);
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                ret = handle_key(&event);
                break;
            default:
                break;
        }
        if (ret) {
            *err = "Event Queue is full";
            return (1);
        }
    }
    return (0);
}
