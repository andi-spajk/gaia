/** generator.h

Prototypes for code generation and label resolution/assembly.

*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>

#include "lexer.h"
#include "opcode.h"
#include "symbol_table.h"

int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int pc);
int resolve_label_ref(FILE *f, struct Instruction *instr, struct Token *label,
                      int operand_status, struct SymbolTable *symtab, int pc);

#endif