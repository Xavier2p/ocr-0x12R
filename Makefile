CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -std=c99 -g -rdynamic
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm -g


BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main

clear:
	find . -type d -empty -delete

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLIBS)

image_traitment:
	make -C src/image_traitment

solver:
	make -C src/sudoku_solver

neural:
	make -C src/neural_network

gui:
	make -C src/gui

format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

test:
	-mv images/* .
	./main image_01.jpeg && \
	./main image_02.jpeg && \
	./main image_03.jpeg && \
	./main image_04.jpeg && \
	./main image_05.jpeg && \
	./main image_06.jpeg && \
	mv image_0* images/
	feh *.jpeg

valim:
	valgrind ./main image_01.jpeg

valim1:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  -s ./main image_01.jpeg

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	make -C src/gui clean
	make -C src/neural_network clean
	make -C src/image_traitment clean
	make -C src/sudoku_solver clean
	@${RM} main
	@rm -rf $(BUILD)
