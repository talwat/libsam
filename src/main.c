#include <SDL.h>
#include <SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "sam.h"
#include "reciter.h"

extern int debug = 0;
static int pos = 0;

// Audio callback: mix audio from SAM buffer into SDL stream
void MixAudio(void *unused, Uint8 *stream, int len)
{
    int bufferpos = GetBufferLength();
    Uint8 *buffer = (Uint8 *)GetBuffer();

    memset(stream, 128, len);

    if (pos >= bufferpos)
        return;

    if ((bufferpos - pos) < len)
        len = bufferpos - pos;

    SDL_MixAudioFormat(stream, buffer + pos, AUDIO_U8, len, 64);
    pos += len;
}

// Init audio system
void InitAudio()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "error: SDL_Init failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_AudioSpec fmt;
    fmt.freq = 22050;
    fmt.format = AUDIO_U8;
    fmt.channels = 1;
    fmt.samples = 2048;
    fmt.callback = MixAudio;
    fmt.userdata = NULL;

    if (SDL_OpenAudio(&fmt, NULL) < 0)
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_PauseAudio(0); // Start playback
}

// SpeakText blocks until audio done playing
int SpeakText(const char *text)
{
    char input[256] = {0};
    strncpy(input, text, 255);

    if (!TextToPhonemes((unsigned char *)input))
        return 1;

    SetInput(input);
    SAMMain();

    pos = 0;

    int bufferpos = GetBufferLength();

    // Wait until the buffer has finished playing
    while (pos < bufferpos)
    {
        SDL_Delay(10);
    }

    return 0;
}

void CloseAudio()
{
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    SDL_Quit();
}