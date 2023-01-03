CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/test_parser.o $(OBJ)/parser.o $(OBJ)/lexer.o $(OBJ)/opcode.o \
      $(OBJ)/symbol_table.o $(OBJ)/prime.o $(OBJ)/assemble.o $(OBJ)/error.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

test_parser: $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_parser

$(OBJ)/test_parser.o: test_parser.c exhaustive_lines.h
	$(CC) $(CFLAGS) $< -c
	mv test_parser.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_parser $(DEP)