#include <SDL.h>
#include <SDL_audio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "sam.h"
#include "reciter.h"

extern int debug = 0;

static SDL_AudioDeviceID device = 0;

int InitSAMAudio()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_AudioSpec want = {0};
    want.freq = 22050;
    want.format = AUDIO_U8;
    want.channels = 1;
    want.samples = 4096;

    device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (!device)
    {
        fprintf(stderr, "SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_PauseAudioDevice(device, 0);
    return 0;
}

int play_sam(int volume, unsigned char *buffer, int length)
{
    SDL_LockAudioDevice(device);

    // unsigned char *dst = malloc(length);
    // SDL_MixAudioFormat(dst, buffer, AUDIO_U8, length, volume);
    // free(buffer);

    if (SDL_QueueAudio(device, buffer, length) < 0)
    {
        fprintf(stderr, "SDL_QueueAudio failed: %s\n", SDL_GetError());
        SDL_UnlockAudioDevice(device);
        return -1;
    }

    SDL_UnlockAudioDevice(device);

    return 0;
}

int trim_sam(unsigned char *buffer, int length)
{
    int silenceCount = 0;

    for (int i = 0; i < length; i++)
    {
        if (buffer[i] == 128)
        {
            silenceCount++;
            if (silenceCount >= 2048)
            {
                return i - silenceCount + 1;
            }
        }
        else
        {
            silenceCount = 0;
        }
    }

    return length;
}

int GenerateSAM(char *text, unsigned char **dest, int *len)
{
    char input[256] = {0};
    strncpy(input, text, 255);

    if (!TextToPhonemes((unsigned char *)input))
        return 1;

    SetInput(input);
    SAMMain();

    *dest = GetBuffer();
    int raw_len = GetBufferLength();
    *len = trim_sam(*dest, raw_len);

    return 0;
}

void CloseSAMAudio()
{
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    SDL_Quit();
}

void SpeakSAM(int volume, char *text)
{
    int len;
    unsigned char *buf;

    GenerateSAM(text, &buf, &len);
    play_sam(volume, buf, len);
}