# Makefile
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

all: init gui clear

clear:
	find . -type d -empty -delete


init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

gui: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS)

format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

$(build)/%.o : %.c
	$(cc) $(cflags) -c $< -o $@ $(ldlflags) $(cppflags) $(ldlibs)

clean:
	@${RM} *.o *.d
	@${RM} $(OBJ) $(EXEC) $(TARGET)
	@rm -rf $(BUILD)


# END OF MAKEFILE
