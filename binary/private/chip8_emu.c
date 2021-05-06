#include <stdio.h>

#include "renderer.h"
#include "emulator.h"

static void
shutdown(char const *msg)
{
    if (msg) {
        puts(msg);
    }
    renderer_destroy();
}

int
main()
{
    char const *err = NULL;

    if (renderer_init(&err)) {
        shutdown(err);
        return (1);
    }
    if (emu_load_rom(NULL, EMU_RT_CHIP_8, &err)) {
        shutdown(err);
    }
    puts("Nya!");
    shutdown(NULL);
    return (0);
}