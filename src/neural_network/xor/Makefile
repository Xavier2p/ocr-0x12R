# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -fsanitize=address
LDLIBS = -lm

SRC = xor.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

BUILD := build
EXEC := xor

all: init makexor

clear:
	find . -type d -empty -delete


init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))


makexor: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^ $(LDLIBS)


$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)


clean:
	${RM} ${OBJ}
	rm -rf $(BUILD)
	$(RM) $(EXEC)
	${RM} ${DEP}
	${RM} xor

# END
