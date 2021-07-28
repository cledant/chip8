#include "audio.h"

#include <math.h>

#include "SDL.h"

#define DEFAULT_SAMPLE_RATE 48000
#define DEFAULT_FRAME_PER_BUFFER 2048
#define DEFAULT_AMPLITUDE 1

/*
 * Audio device variables
 */
static uint32_t audio_device_id;
static uint32_t audio_sample_rate;
static uint32_t audio_amplitude = DEFAULT_AMPLITUDE;
static double audio_buzzer_freq;
static double audio_sample_per_sine;

/*
 * Buzzer buffer related variables
 */
static float *audio_buzzer_buffer;
static uint32_t audio_buffer_pos;
static int audio_is_playing;

static void
audio_play_callback(void *user_data, Uint8 *stream, int len)
{
    (void)user_data;
    float *fs = (float *)stream;
    int fs_len = len / sizeof(float);
    for (int i = 0; i < fs_len; ++i) {
        fs[i] = audio_buzzer_buffer[audio_buffer_pos];
        audio_buffer_pos =
          (audio_buffer_pos + 1) % (uint32_t)audio_sample_per_sine;
    }
}

int
generate_buzzer(char const **err)
{
    if (audio_buzzer_buffer) {
        free(audio_buzzer_buffer);
        audio_buzzer_buffer = NULL;
    }

    if (!(audio_buzzer_buffer =
            (float *)malloc((uint32_t)audio_sample_per_sine * sizeof(float)))) {
        *err = "Failed to allocate buzzer buffer";
        return (1);
    }
    for (uint32_t i = 0; i < (uint32_t)audio_sample_per_sine; ++i) {
        audio_buzzer_buffer[i] =
          (sin((i / audio_sample_per_sine) * M_PI * 2.0)) * audio_amplitude;
    }
    return (0);
}

/*
 * Public API
 */
int
audio_init(char const **err)
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        *err = "Failed to init audio";
        return (1);
    }
    SDL_AudioSpec wanted = { .freq = DEFAULT_SAMPLE_RATE,
                             .format = AUDIO_F32,
                             .samples = DEFAULT_FRAME_PER_BUFFER,
                             .channels = 1,
                             .callback = audio_play_callback,
                             .userdata = NULL };
    SDL_AudioSpec get;

    if (!(audio_device_id = SDL_OpenAudioDevice(
            NULL,
            SDL_FALSE,
            &wanted,
            &get,
            SDL_AUDIO_ALLOW_CHANNELS_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
              SDL_AUDIO_ALLOW_SAMPLES_CHANGE))) {
        *err = "Failed to get audio device";
        return (1);
    }
    audio_sample_rate = get.freq;
    return (0);
}

void
audio_destroy()
{
    if (audio_device_id) {
        SDL_PauseAudioDevice(audio_device_id, 1);
        SDL_CloseAudioDevice(audio_device_id);
    }
    if (audio_buzzer_buffer) {
        free(audio_buzzer_buffer);
        audio_buzzer_buffer = NULL;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

int
audio_set_buzzer_params(double freq, uint32_t amplitude, char const **err)
{
    (void)err;
    audio_buzzer_freq = fabs(freq);
    audio_amplitude = amplitude;
    audio_sample_per_sine = (double)audio_sample_rate / audio_buzzer_freq;
    audio_buffer_pos = 0;
    if (generate_buzzer(err)) {
        return (1);
    }
    return (0);
}

void
audio_play_buzzer()
{
    if (audio_is_playing) {
        return;
    }
    SDL_PauseAudioDevice(audio_device_id, 0);
    audio_is_playing = 1;
}

void
audio_stop_buzzer()
{
    SDL_PauseAudioDevice(audio_device_id, 1);
    audio_buffer_pos = 0;
    audio_is_playing = 0;
}
