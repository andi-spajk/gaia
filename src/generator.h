/** generator.h

Prototypes for code generation and label resolution/assembly.

*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>

#include "forward_reference.h"
#include "lexer.h"
#include "opcode.h"
#include "symbol_table.h"

int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int curr_pc);
int resolve_label_ref(FILE *f, struct Lexer *lexer, struct Instruction *instr,
                      struct Token *label, int operand_status,
                      struct SymbolTable *symtab, int curr_pc);
int resolve_forward_ref(FILE *f, struct ForwardRef *ref, struct Lexer *lexer,
                        struct SymbolTable *symtab);


#endif