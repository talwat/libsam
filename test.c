#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL.h>
#include <SDL_audio.h>

#include "libsam.h"

int main() {
    InitSAMAudio();
    SpeakSAM(128, "HELLO WORLD.");
    sleep(1); // Wait for audio to finish
    CloseSAMAudio();
    return 0;
}