# Makefile
CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -rdynamic
LDFLAGS=
LDLIBS=`pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm

# The name of the executable
TARGET=gui

# The source files
SRC=utilis_image.c gui.c main.c gui_tools.c
OBJ=$(SRC:.c=.o)
EXE=${SRC:.c=}

main:
	${CC} ${CFLAGS} ${SRC} -o ${TARGET} ${LDLIBS}


format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: all

clean:
	${RM} $(OBJ) $(EXE) $(TARGET)
	@${RM} *_resized*

# END OF MAKEFILE
