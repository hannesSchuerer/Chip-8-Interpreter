OBJS = main.c chip.c window.c

OBJ_NAME = Emulator

all : $(OBJS)
	gcc $(OBJS) -IC:\code\SDL2\i686-w64-mingw32\include -LC:\code\SDL2\i686-w64-mingw32\lib -Wall -Wl,-subsystem,console -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)