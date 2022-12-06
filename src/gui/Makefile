CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -rdynamic
LDFLAGS=
LDLIBS=`pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm

# The name of the executable
BUILD := build
EXEC := main

# The source files
SRC = resize.c utilis_image.c gui.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = ${SRC:%.c=$(BUILD)/%.d

all: init ui clear

init:
	@$(shell mkdir -p $(BUILD))
	@$(shell mkdir -p $(SRC:%=$(BUILD)/%))

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
	${RM} *_resized*
