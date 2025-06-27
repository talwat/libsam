#ifndef SAM_H
#define SAM_H

#include <SDL.h>
#include <SDL_audio.h>
#include <stdint.h>

extern int debug;

int InitSAMAudio();
int GenerateSAM(char *text, unsigned char **dest, int *len);
void CloseSAMAudio();
void SpeakSAM(char *text);

void SetInput(char *_input);
void SetSpeed(unsigned char _speed);
void SetPitch(unsigned char _pitch);
void SetMouth(unsigned char _mouth);
void SetThroat(unsigned char _throat);
void EnableSingmode();
void EnableDebug();
int SAMMain();
char *GetBuffer();
int GetBufferLength();
int TextToPhonemes(unsigned char *input);

#endif // SAM_H