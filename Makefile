OBJS = reciter.o sam.o render.o main.o

CC = gcc

# libsdl present
CFLAGS = -Wall -Os -DUSESDL `sdl2-config --cflags`
LFLAGS = `sdl2-config --libs`

# no libsdl present
# CFLAGS = -Wall -Os
# LFLAGS =

LIBNAME = libsam.a

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	ar rcs $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(LIBNAME)