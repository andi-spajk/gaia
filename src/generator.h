#ifndef GENERATOR_H
#define GENERATOR_H

#include "lexer.h"
#include "opcode.h"

int calc_num_bytes(int operand_bytes);
int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int pc);

#endif