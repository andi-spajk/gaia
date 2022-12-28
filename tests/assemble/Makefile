CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_assemble.o $(OBJ)/assemble.o $(OBJ)/lexer.o $(OBJ)/opcode.o \
      $(OBJ)/parser.o $(OBJ)/symbol_table.o $(OBJ)/prime.o

test_assemble: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_assemble

$(OBJ)/test_assemble.o: test_assemble.c
	$(CC) $(CFLAGS) $< -c
	mv test_assemble.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_assemble $(DEP)