CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/test_generator.o $(OBJ)/generator.o $(OBJ)/opcode.o \
      $(OBJ)/assemble.o $(OBJ)/lexer.o $(OBJ)/symbol_table.o $(OBJ)/prime.o \
      $(OBJ)/parser.o $(OBJ)/forward_reference.o $(OBJ)/error.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

test_generator: $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_generator

$(OBJ)/test_generator.o: test_generator.c
	$(CC) $(CFLAGS) $< -c
	mv test_generator.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_generator $(DEP) *.out