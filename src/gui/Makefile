# Makefile
CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -rdynamic
LDFLAGS=
LDLIBS=`pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm

# The name of the executable
TARGET=gui

# The source files
SRC=utilis_image.c gui.c main.c gui_tools.c launcher.c preprocess.c
OBJ=$(SRC:.c=.o)
EXE=${SRC:.c=}


$(TARGET): $(OBJ)
	${CC} ${CFLAGS} -o $@ $^ ${LDLIBS}


format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;


all: $(TARGET)


.PHONY: all


clean:
	${RM} $(OBJ) $(EXE) $(TARGET)

# END OF MAKEFILE
