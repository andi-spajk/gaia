CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/test_opcode.o $(OBJ)/opcode.o $(OBJ)/assemble.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

test_opcode: $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -o test_opcode

$(OBJ)/test_opcode.o: test_opcode.c
	$(CC) $(CFLAGS) $< -c
	mv test_opcode.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_opcode $(DEP)