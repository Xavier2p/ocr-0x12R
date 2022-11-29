# Makefile

CC = gcc
CFLAGS = -g
LDLIBS = -lm

SRC = ${shell find -name "*.c"}
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
	rm -rf $(BUILD)
	${RM} ${EXEC}
	${RM} ${DEP}
	${RM} main

# END
