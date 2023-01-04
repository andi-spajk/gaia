CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/parser.o $(OBJ)/lexer.o $(OBJ)/opcode.o $(OBJ)/symbol_table.o \
      $(OBJ)/prime.o $(OBJ)/assemble.o $(OBJ)/error.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

all: test_parser1 test_parser2

test_parser1: $(OBJ)/test_parser1.o $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_parser1

$(OBJ)/test_parser1.o: test_parser1.c
	$(CC) $(CFLAGS) $< -c
	mv test_parser1.o $(OBJ)

test_parser2: $(OBJ)/test_parser2.o $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_parser2

$(OBJ)/test_parser2.o: test_parser2.c exhaustive_lines.h
	$(CC) $(CFLAGS) $< -c
	mv test_parser2.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_parser1 test_parser2 $(OBJ)/test_parser1.o $(OBJ)/test_parser2.o\
	      $(DEP)