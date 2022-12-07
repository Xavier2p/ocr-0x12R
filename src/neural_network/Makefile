# Makefile

CC = gcc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address -O3
LDLIBS = -lm
LDFLAGS = -fsanitize=address

SRC = load_numeric.c main.c neural_network.c save_load.c training.c
OBJ = ${SRC:%.c=$(BUILD)/%.o}
DEP = ${SRC:%.c=$(BUILD)/%.d}

BUILD := build
EXEC := neural

all: init neural

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))


neural: $(OBJ)
	${CC} ${CFLAGS} ${LDFLAGS} $^ -o $(EXEC) ${LDLIBS}


$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)

clean:
	@rm -rf $(BUILD)
	@${RM} ${EXEC} ${DEP} ${EXEC}

# END
