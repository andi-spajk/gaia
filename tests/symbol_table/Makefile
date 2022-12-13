CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_symbol_table.o $(OBJ)/symbol_table.o $(OBJ)/prime.o

test_symbol_table: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_symbol_table

$(OBJ)/test_symbol_table.o: test_symbol_table.c
	$(CC) $(CFLAGS) $^ -c
	mv test_symbol_table.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_symbol_table $(DEP)
	# don't delete unity.o