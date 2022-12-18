/*
TOKEN SEQUENCES:
InstructionTree
	instruction
		open parenthesis
			operand
				comma
					x_register
						close parenthesis
							null
				close parenthesis
					comma
						y_register
							null
					null
		operand
			comma
				x_register
				y_register
					null
			null
		immediate
			operand
				null

LabelTree
	label
		equal_sign
			literal
				null
		InstructionTree
		null
*/

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "opcode.h"
#include "symbol_table.h"

#define PARSER_SUCCESS           1
#define BRANCH_OPERAND           2
#define JUMP_OPERAND             3
#define BRANCH_FORWARD_REFERENCE 4
#define JUMP_FORWARD_REFERENCE   5

int parse_instr_tree(struct Token **seq, int index);
int parse_label_tree(struct Token **seq, int index);
int parse_token_sequence(struct Lexer *lexer);
int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc);
int parse_label_operand(struct Token *operand, struct Instruction *instr,
                        struct SymbolTable *symtab);
struct Token *get_operand(struct Lexer *lexer);
// int parse_operand(struct Lexer *lexer, struct SymbolTable *symtab,
//                   struct Instruction *instr, int pc);

#endif