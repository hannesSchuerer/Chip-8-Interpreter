OBJS = main.c chip.c window.c

OBJ_NAME = Emulator

SDL_LIB = -LC:\code\SDL2\i686-w64-mingw32\lib
SDL_INCLUDE = -IC:\code\SDL2\i686-w64-mingw32\include

all : $(OBJS)
	gcc $(OBJS) $(SDL_INCLUDE) $(SDL_LIB) -Wall -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)