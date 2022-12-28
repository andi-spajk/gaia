CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_lexer.o $(OBJ)/lexer.o $(OBJ)/opcode.o $(OBJ)/assemble.o

test_lexer: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -o test_lexer

$(OBJ)/test_lexer.o: test_lexer.c
	$(CC) $(CFLAGS) $^ -c
	mv test_lexer.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_lexer $(DEP)