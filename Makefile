CC = gcc
CFLAGS = -Wall -Wextra -Werror `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -O3 -std=c99 -pedantic
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lSDL2 -lm

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

neural_network:
	make -C src/neural_network

solver:
	make -C src/sudoku_solver

neural:
	make -C src/neural_network

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
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  -s ./main image_01.jpeg

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	${RM} main
	${RM} res_*
	${RM} ${OBJ}
	rm -rf $(BUILD)
	${RM} ${EXE}
	${RM} res_*
	${RM} .ut*
	${RM} .Make*
	${RM} .main*
	${RM} .sob*
