/** parser.h

Prototypes that parse a sequence of tokens for syntax errors, determines
addressing mode via bitmasks, insert labels into the symbol table, and locates
forward references to be resolved later.

*/

/*
TOKEN SEQUENCES:
InstructionTree
	instruction
		open parenthesis
			label | literal
				comma
					x_register
						close parenthesis
							null
				close parenthesis
					comma
						y_register
							null
					null
		label | literal
			comma
				x_register
				y_register
					null
			null
		immediate
			label | literal
				null
		null

LabelTree
	label
		equal_sign | equ_directive
			literal
				null
		InstructionTree
		null

DirectiveTree
	define_directive
		label
			literal
				null
	org_directive
		literal
			null
	end_directive
		null

BaseTree
	base
		equal_sign
			literal
				null
*/

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "opcode.h"
#include "symbol_table.h"

// return codes
#define PARSER_SUCCESS           1
#define BRANCH_OPERAND           2
#define JUMP_OPERAND             3
#define BRANCH_FORWARD_REFERENCE 4
#define JUMP_FORWARD_REFERENCE   5
#define FORWARD_REFERENCE        0xFFFF

int parse_instr_tree(struct Lexer *lexer, int index);
int parse_label_tree(struct Lexer *lexer, int index);
int parse_directive_tree(struct Lexer *lexer, int index);
int parse_base_tree(struct Lexer *lexer, int index);
int parse_line(struct Lexer *lexer);

int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc);
int parse_label_operand(struct Lexer *lexer, struct Instruction *instr,
                        struct Token *operand, struct SymbolTable *symtab);
int parse_operand(struct Lexer *lexer, struct Instruction *instr,
                  struct Token *operand, struct SymbolTable *symtab);
struct Token *find_operand(struct Lexer *lexer);

int apply_masks(struct Lexer *lexer, int curr_field);
int parse_forward_ref_addr_mode(struct Lexer *lexer, struct Instruction *instr);
int parse_addr_mode(struct Lexer *lexer, struct Instruction *instr,
                    struct Token *operand, int operand_status);

#endif