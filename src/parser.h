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
#include "symbol_table.h"

#define PARSER_SUCCESS 1

int parse_instr_tree(struct Token **seq, int index);
int parse_label_tree(struct Token **seq, int index);
int parse_token_sequence(struct Lexer *lexer);
int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc);

#endif