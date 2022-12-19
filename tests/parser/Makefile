CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_parser.o $(OBJ)/parser.o $(OBJ)/lexer.o $(OBJ)/opcode.o \
      $(OBJ)/symbol_table.o $(OBJ)/prime.o

test_parser: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_parser

$(OBJ)/test_parser.o: test_parser.c exhaustive_lines.h
	$(CC) $(CFLAGS) $< -c
	mv test_parser.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_parser $(DEP)