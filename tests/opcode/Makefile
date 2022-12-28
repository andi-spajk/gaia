CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_opcode.o $(OBJ)/opcode.o $(OBJ)/assemble.o

test_opcode: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -o test_opcode

$(OBJ)/test_opcode.o: test_opcode.c
	$(CC) $(CFLAGS) $^ -c
	mv test_opcode.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_opcode $(DEP)