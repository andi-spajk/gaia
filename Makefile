ifeq ($(OS), Windows_NT)
	GAIA = gaia.exe
	RM = del
	MV = move
else
	GAIA = gaia
	RM = rm -f
	MV = mv
endif

CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11

OBJ = obj
SRC = src
CFLAGS += -I$(SRC)

DEP = $(OBJ)/main.o              \
      $(OBJ)/assemble.o          \
      $(OBJ)/error.o             \
      $(OBJ)/forward_reference.o \
      $(OBJ)/generator.o         \
      $(OBJ)/lexer.o             \
      $(OBJ)/opcode.o            \
      $(OBJ)/parser.o            \
      $(OBJ)/prime.o             \
      $(OBJ)/symbol_table.o

HEADERS = $(SRC)/addressing_modes.h  \
          $(SRC)/assemble.h          \
          $(SRC)/bitfields.h         \
          $(SRC)/error.h             \
          $(SRC)/forward_reference.h \
          $(SRC)/generator.h         \
          $(SRC)/lexer.h             \
          $(SRC)/opcode.h            \
          $(SRC)/parser.h            \
          $(SRC)/prime.h             \
          $(SRC)/symbol_table.h

$(GAIA): $(DEP)
	$(CC) $(CFLAGS) $^ -lm -o $(GAIA)

$(OBJ)/main.o: $(SRC)/main.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c
	$(MV) main.o $(OBJ)

$(OBJ)/assemble.o: $(SRC)/assemble.c $(SRC)/addressing_modes.h $(SRC)/error.h
	$(CC) $(CFLAGS) $< -c
	$(MV) assemble.o $(OBJ)

$(OBJ)/error.o: $(SRC)/error.c $(SRC)/error.h
	$(CC) $(CFLAGS) $< -c
	$(MV) error.o $(OBJ)

$(OBJ)/forward_reference.o: $(SRC)/forward_reference.c $(SRC)/error.h \
                            $(SRC)/forward_reference.h $(SRC)/lexer.h \
                            $(SRC)/opcode.h
	$(CC) $(CFLAGS) $< -c
	$(MV) forward_reference.o $(OBJ)

$(OBJ)/generator.o: $(SRC)/generator.c $(SRC)/addressing_modes.h \
                    $(SRC)/error.h $(SRC)/forward_reference.h $(SRC)/lexer.h \
                    $(SRC)/opcode.h $(SRC)/parser.h $(SRC)/symbol_table.h
	$(CC) $(CFLAGS) $< -c
	$(MV) generator.o $(OBJ)

$(OBJ)/lexer.o: $(SRC)/lexer.c $(SRC)/error.h $(SRC)/lexer.h $(SRC)/opcode.h
	$(CC) $(CFLAGS) $< -c
	$(MV) lexer.o $(OBJ)

$(OBJ)/opcode.o: $(SRC)/opcode.c $(SRC)/assemble.h $(SRC)/bitfields.h \
                 $(SRC)/error.h $(SRC)/opcode.h
	$(CC) $(CFLAGS) $< -c
	$(MV) opcode.o $(OBJ)

$(OBJ)/parser.o: $(SRC)/parser.c $(SRC)/addressing_modes.h $(SRC)/error.h \
                 $(SRC)/lexer.h $(SRC)/opcode.h $(SRC)/parser.h \
                 $(SRC)/symbol_table.h
	$(CC) $(CFLAGS) $< -c
	$(MV) parser.o $(OBJ)

$(OBJ)/prime.o: $(SRC)/prime.c
	$(CC) $(CFLAGS) $< -c
	$(MV) prime.o $(OBJ)

$(OBJ)/symbol_table.o: $(SRC)/symbol_table.c $(SRC)/error.h $(SRC)/prime.h \
                       $(SRC)/symbol_table.h
	$(CC) $(CFLAGS) $< -c
	$(MV) symbol_table.o $(OBJ)

clean:
	$(RM) $(GAIA) $(DEP)