/** parser.c

Parser for the assembler. Analyzer a sequence of tokens prepared by the lexer
to locate syntax errors. Label declarations are inserted into the symbol
table, and forward references and saved. Addresing modes and opcodes are also
determined and saved into the Instruction struct.

The parser determines syntax errors such as illegal sequences of tokens, or
instructions with incompatible addressing modes. Other errors include redefining
label constants, missing label declarations, or illegal forward references.

See parser.h for full diagram of valid token sequences.
*/

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"

/* parse_indirect_operand_tree()
	@seq            ptr to sequence of tokens from a lexer
	@index          index of operand token

	@return         success or error code

	Parse the indirect operand for syntax errors.
*/
int parse_indirect_operand_tree(struct Token **seq, int index)
{
	// ,
	if (seq[index+1]->type == TOKEN_COMMA) {
		// ,X
		if (seq[index+2]->type == TOKEN_X_REGISTER) {
			// ,X)
			if (seq[index+3]->type == TOKEN_CLOSE_PARENTHESIS) {
				if (seq[index+4]->type == TOKEN_NULL)
					return PARSER_SUCCESS;
			}
		}
	}
	// )
	else if (seq[index+1]->type == TOKEN_CLOSE_PARENTHESIS) {
		// ),
		if (seq[index+2]->type == TOKEN_COMMA) {
			// ),Y
			if (seq[index+3]->type == TOKEN_Y_REGISTER) {
				if (seq[index+4]->type == TOKEN_NULL)
					return PARSER_SUCCESS;
			}
		}
		// )
		else if (seq[index+2]->type == TOKEN_NULL) {
			return PARSER_SUCCESS;
		}
	}
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_instr_tree()
	@seq            ptr to sequence of tokens from a lexer
	@index          index of instruction token

	@return         success or error code

	Parse the instruction token and its operand for syntax errors.
*/
int parse_instr_tree(struct Token **seq, int index)
{
	// (
	if (seq[index+1]->type == TOKEN_OPEN_PARENTHESIS) {
		// (OPERAND
		if (seq[index+2]->type == TOKEN_LABEL ||
		    seq[index+2]->type == TOKEN_LITERAL)
			return parse_indirect_operand_tree(seq, index+2);
	}
	// OPERAND
	else if (seq[index+1]->type == TOKEN_LABEL ||
	         seq[index+1]->type == TOKEN_LITERAL) {
		if (seq[index+2]->type == TOKEN_NULL)
			return PARSER_SUCCESS;
	}
	// #
	else if (seq[index+1]->type == TOKEN_IMMEDIATE) {
		// #OPERAND
		// immediate labels are also legal
		if (seq[index+2]->type == TOKEN_LITERAL ||
		    seq[index+2]->type == TOKEN_LABEL) {
			if (seq[index+3]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	}
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_label_tree()
	@seq            ptr to sequence of tokens from a lexer
	@index          index of label token

	@return         success or error code

	Parse the label line and detect syntax errors.
*/
int parse_label_tree(struct Token **seq, int index)
{
	// =
	if (seq[index+1]->type == TOKEN_EQUAL_SIGN) {
		// = $A0
		if (seq[index+2]->type == TOKEN_LITERAL) {
			if (seq[index+3]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	}
	// LDA
	else if (seq[index+1]->type == TOKEN_INSTRUCTION) {
		return parse_instr_tree(seq, index+1);
	}
	// just a declaration of label
	else if (seq[index+1]->type == TOKEN_NULL) {
		return PARSER_SUCCESS;
	}
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_token_sequence()
	@lexer          ptr to Lexer struct

	@return         success or error code

	Check @lexer's token sequence for syntax errors.
*/
int parse_token_sequence(struct Lexer *lexer)
{
	int index = 0;
	struct Token **seq = lexer->sequence;
	if (seq[index]->type == TOKEN_INSTRUCTION)
		return parse_instr_tree(seq, index);
	else if (seq[index]->type == TOKEN_LABEL)
		return parse_label_tree(seq, index);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_label_declaration()
	@lexer          ptr to Lexer struct
	@symtab         ptr to symbol table
	@pc             current program counter

	@return         success or error code

	Parse a source line that begins with a label and insert the label into
	the symbol table. The label's value is either the current program
	counter location, or a constant.
*/
int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc)
{
	struct Token **seq = lexer->sequence;
	struct Token *label = seq[0];
	int found = search_symbol(symtab, label->str);

	if (seq[1]->type != TOKEN_EQUAL_SIGN) {
		// address label
		if (found != ERROR_SYMBOL_NOT_FOUND) {
			return ERROR_LABEL_REDEFINITION;
		} else {
			label->value = pc;
			return insert_symbol(symtab, label->str, label->value);
		}
	} else if (seq[1]->type == TOKEN_EQUAL_SIGN) {
		// constant label
		if (found != ERROR_SYMBOL_NOT_FOUND) {
			return ERROR_LABEL_REDEFINITION;
		} else {
			// 0     1 2
			// LABEL = $00
			label->value = seq[2]->value;
			return insert_symbol(symtab, label->str, label->value);
		}
	} else if (seq[1]->type == TOKEN_NULL) {
		// lone label declaration
		return PARSER_SUCCESS;
	}
	return ERROR_UNKNOWN;
}