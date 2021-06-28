#ifndef CHIP8_EMU_AUDIO_H
#define CHIP8_EMU_AUDIO_H

#include <stdint.h>

int audio_init(char const **err);
void audio_destroy();
int audio_set_buzzer_params(double freq, uint32_t amplitude, char const **err);
void audio_play_buzzer();
void audio_stop_buzzer();

#endif // CHIP8_EMU_AUDIO_H
