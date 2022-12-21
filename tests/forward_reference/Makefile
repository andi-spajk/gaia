CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_forward_reference.o $(OBJ)/forward_reference.o \
      $(OBJ)/opcode.o $(OBJ)/lexer.o $(OBJ)/parser.o \
      $(OBJ)/symbol_table.o $(OBJ)/prime.o

test_forward_reference: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_forward_reference

$(OBJ)/test_forward_reference.o: test_forward_reference.c
	$(CC) $(CFLAGS) $< -c
	mv test_forward_reference.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_forward_reference $(DEP)