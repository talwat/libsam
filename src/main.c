#include <SDL.h>
#include <SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "sam.h"
#include "reciter.h"

static SDL_AudioDeviceID audio_device;
static SDL_AudioSpec fmt;
static pthread_mutex_t audio_mutex = PTHREAD_MUTEX_INITIALIZER;

static Uint8 *audioBuffer = NULL;
static int audioBufferLen = 0;
static int debug = 0;
static int pos = 0;

static void MixAudio(void *userdata, Uint8 *stream, int len)
{
    pthread_mutex_lock(&audio_mutex);

    memset(stream, 128, len);

    if (pos >= audioBufferLen) {
        pthread_mutex_unlock(&audio_mutex);
        return;
    }

    int remaining = audioBufferLen - pos;
    if (remaining < len)
        len = remaining;

    SDL_MixAudioFormat(stream, audioBuffer + pos, AUDIO_U8, len, 48);
    pos += len;

    pthread_mutex_unlock(&audio_mutex);
}

void InitSAMAudio(void)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Unable to init SDL audio: %s\n", SDL_GetError());
        exit(1);
    }

    fmt.freq = 22050;
    fmt.format = AUDIO_U8;
    fmt.channels = 1;
    fmt.samples = 2048;
    fmt.callback = MixAudio;
    fmt.userdata = NULL;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &fmt, NULL, 0);
    if (!audio_device) {
        fprintf(stderr, "Unable to open SDL audio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_PauseAudioDevice(audio_device, 0);
}

void SpeakText(const char *text)
{
    char input[256] = {0};
    strncpy(input, text, 255);

    if (!TextToPhonemes((unsigned char *)input)) {
        fprintf(stderr, "TextToPhonemes failed\n");
        return;
    }

    pthread_mutex_lock(&audio_mutex);

    SetInput(input);
    SAMMain();

    audioBuffer = GetBuffer();
    audioBufferLen = GetBufferLength();
    pos = 0;

    pthread_mutex_unlock(&audio_mutex);
}