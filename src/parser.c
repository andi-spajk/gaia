/** parser.c

Parser for the assembler. Analyzer a sequence of tokens prepared by the lexer
to locate syntax errors. Label declarations are inserted into the symbol
table, and forward references and saved. Addresing modes and opcodes are also
determined and saved into the Instruction struct.

The parser determines syntax errors such as illegal sequences of tokens, or
instructions with incompatible addressing modes. Other errors include redefining
label constants, missing label declarations, or illegal forward references.

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

#include "error.h"
#include "lexer.h"
#include "parser.h"

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

// int parse_label_tree(const struct Lexer *lexer, int index)

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
	// else if (seq[index]->type == TOKEN_LABEL)
	// 	return parse_label_tree(seq, index);
	return ERROR_ILLEGAL_SEQUENCE;
}