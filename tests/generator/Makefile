CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_generator.o $(OBJ)/generator.o $(OBJ)/opcode.o \
      $(OBJ)/assemble.o $(OBJ)/lexer.o $(OBJ)/symbol_table.o \
      $(OBJ)/prime.o $(OBJ)/parser.o $(OBJ)/forward_reference.o

test_generator: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_generator

$(OBJ)/test_generator.o: test_generator.c
	$(CC) $(CFLAGS) $^ -c
	mv test_generator.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_generator $(DEP)