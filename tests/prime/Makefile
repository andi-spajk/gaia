CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = ../../obj
SRC = ../../src
CFLAGS += -I$(SRC)

UNITY = ../../unity/unity.o
DEP = $(OBJ)/test_prime.o $(OBJ)/prime.o

test_prime: $(DEP)
	$(CC) $(CFLAGS) $^ $(UNITY) -lm -o test_prime

$(OBJ)/test_prime.o: test_prime.c
	$(CC) $(CFLAGS) $^ -c
	mv test_prime.o $(OBJ)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) $< -c
	mv *.o $(OBJ)

clean:
	rm -f test_prime $(OBJ)