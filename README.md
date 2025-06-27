# libsam

> [!WARNING]
> This library is still under heavy development and is prone to causing
> loud screeching noises! Do not use it yet!

A library version of [SAM](https://github.com/s-macke/SAM).

Don't ask me how it works, I have literally no idea.
This version of SAM has been poorly altered to not rupture my
eardrums and use SDL 2.

There is also an included header file at [libsam.h](/libsam.h).

> [!NOTE]
> While the header file is provided as `libsam.h` in the repo, it should
> be saved in the source tree as `sam.h`. It isn't provided as such to avoid
> confusion with the already existing `sam.h` elsewhere.

## Example

```c
#include "sam.h"

int main() {
    InitSAMAudio();
    SpeakSAM("HELLO WORLD.");
    CloseSAMAudio();
}
```
