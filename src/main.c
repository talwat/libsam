#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "reciter.h"
#include "sam.h"

#ifdef USESDL
#include <SDL.h>
#include <SDL_audio.h>
#endif

int pos = 0;
void MixAudio(void *unused, Uint8 *stream, int len)
{
    int bufferpos = GetBufferLength();
    Uint8 *buffer = (Uint8 *)GetBuffer();
    memset(stream, 128, len);

    if (pos >= bufferpos)
        return;

    if ((bufferpos - pos) < len)
        len = (bufferpos - pos);

    SDL_MixAudioFormat(stream, buffer + pos, AUDIO_U8, len, 64);

    pos += len;
}

void OutputSound()
{
    int bufferpos = GetBufferLength();
    bufferpos /= 50;
    SDL_AudioSpec fmt;

    fmt.freq = 22050;
    fmt.format = AUDIO_U8;
    fmt.channels = 1;
    fmt.samples = 2048;
    fmt.callback = MixAudio;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if (SDL_OpenAudio(&fmt, NULL) < 0)
    {
        printf("Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_PauseAudio(0);

    while (pos < bufferpos)
    {
        SDL_Delay(100);
    }

    SDL_CloseAudio();
}

int debug = 0;