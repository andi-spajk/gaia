CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY_SRC = ../../unity
UNITY_DEP = $(OBJ)/unity.o
DEP = $(OBJ)/test_symbol_table.o $(OBJ)/symbol_table.o $(OBJ)/prime.o
# separate DEP and UNITY_DEP because make clean need not delete unity.o

test_symbol_table: $(DEP) $(UNITY_DEP)
	$(CC) $(CFLAGS) $^ -lm -o test_symbol_table

$(OBJ)/test_symbol_table.o: test_symbol_table.c
	$(CC) $(CFLAGS) $< -c
	mv test_symbol_table.o $(OBJ)

$(UNITY_DEP): $(UNITY_SRC)/unity.c
	$(CC) $(CFLAGS) $< -c
	mv unity.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_symbol_table $(DEP)