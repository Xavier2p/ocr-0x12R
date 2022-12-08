CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -std=c99 -rdynamic
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm

BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLIBS)

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
	./main image_07.jpeg && \
	./main image_08.jpeg && \
	./main image_09.jpeg && \
	./main image_10.jpeg && \
	mv image_* images/
	feh *.jpeg

valim:
	valgrind ./main image_01.jpeg

valim1:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./main image_01.jpeg

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	make -C src/gui clean
	make -C src/neural_network clean
	make -C src/image_traitment clean
	make -C src/sudoku_solver clean
	@${RM} main
	@${RM} -r $(BUILD)
	@${RM} res_*
	@${RM} grid.result
	@${RM} solved*
	@${RM} saved*

