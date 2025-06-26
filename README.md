# libsam

A library version of [SAM](https://github.com/s-macke/SAM).

Don't ask me how it works, I have literally no idea.
This version of SAM has been poorly altered to not rupture my
eardrums and use SDL 2.

## Example? I think?

```c
int speak_text(const char *text) {
  char input[256] = {0};
  strncpy(input, text, 255);

  if (!TextToPhonemes((unsigned char *)input))
    return 1;

  SetInput(input);
  SAMMain();
  OutputSound();

  return 0;
}

int main() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		perror("error: SDL_Init failed\n");
		exit(EXIT_FAILURE);
	}

  speak_text("HELLO.");

	atexit(SDL_Quit);
}
```
