CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/test_forward_reference.o $(OBJ)/forward_reference.o \
      $(OBJ)/opcode.o $(OBJ)/lexer.o $(OBJ)/parser.o $(OBJ)/symbol_table.o \
      $(OBJ)/prime.o $(OBJ)/assemble.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

test_forward_reference: $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_forward_reference

$(OBJ)/test_forward_reference.o: test_forward_reference.c
	$(CC) $(CFLAGS) $< -c
	mv test_forward_reference.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_forward_reference $(DEP)