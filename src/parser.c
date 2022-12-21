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

#include <stddef.h>
#include <stdint.h>

#include "addressing_modes.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
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
		if (seq[index+2]->type == TOKEN_NULL) {
			return PARSER_SUCCESS;
		}
		// OPERAND,
		else if (seq[index+2]->type == TOKEN_COMMA) {
			// OPERAND,X
			// OPERAND,Y
			if (seq[index+3]->type == TOKEN_X_REGISTER ||
			    seq[index+3]->type == TOKEN_Y_REGISTER)
				return PARSER_SUCCESS;
		}
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
	else if (seq[index+1]->type == TOKEN_NULL) {
		return PARSER_SUCCESS;
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

/* parse_line()
	@lexer          ptr to Lexer struct

	@return         success or error code

	Check @lexer's token sequence for syntax errors.
*/
int parse_line(struct Lexer *lexer)
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
		// might be lone label or a label and instr
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
	}
	return ERROR_UNKNOWN;
}

/* parse_label_operand()
	@operand        ptr to operand token in a Lexer sequence
	@instr          ptr to Instruction struct
	@symtab         ptr to symbol table

	@return         operand status, or error code
*/
int parse_label_operand(struct Token *operand, struct Instruction *instr,
                        struct SymbolTable *symtab)
{
	int label_value = search_symbol(symtab, operand->str);
	int branch = is_branch(instr->mnemonic);
	int jump = is_jump(instr->mnemonic);

	if (label_value != ERROR_SYMBOL_NOT_FOUND) {
		// save value in both symtab and sequence token
		// allows for faster lookup when we parse the addressing mode
		operand->value = label_value;
		if (branch)
			return BRANCH_OPERAND;
		else if (jump)
			return JUMP_OPERAND;
		else
			return PARSER_SUCCESS;
	} else {
		// nonexistent symbol may be a forward reference
		// no forward references to constant labels!
		if (branch)
			return BRANCH_FORWARD_REFERENCE;
		else if (jump)
			return JUMP_FORWARD_REFERENCE;
		else
			return ERROR_ILLEGAL_FORWARD_REFERENCE;
	}
	return ERROR_UNKNOWN;
}

/* find_operand()
	@lexer          ptr to Lexer struct

	@return         ptr to operand token in the lexer sequence, or NULL if
	                not found

	Find location of the operand token in a lexer's sequence of tokens.
*/
struct Token *find_operand(struct Lexer *lexer)
{
	struct Token *curr = lexer->sequence[1];
	// start at 2nd token
	// operand will always be 2nd or later
	for (int i = 1; curr->type != TOKEN_NULL; i++) {
		curr = lexer->sequence[i];
		if (curr->type == TOKEN_LABEL ||
		    curr->type == TOKEN_LITERAL)
			return curr;
	}
	return NULL;
}

/* parse_operand()
	@lexer          ptr to Lexer struct
	@instr          ptr to Instruction struct
	@symtab         ptr to symbol table

	@return         operand status, or error code

	Determine whether the operand is a branch/jump instruction and whether
	it is a forward reference. Operands which are neither are ignored.
	Illegal forward references or label references cause errors.

	All operand statuses (from parser.h):
	PARSER_SUCCESS           1
	BRANCH_OPERAND           2
	JUMP_OPERAND             3
	BRANCH_FORWARD_REFERENCE 4
	JUMP_FORWARD_REFERENCE   5
*/
int parse_operand(struct Lexer *lexer, struct Instruction *instr,
                  struct SymbolTable *symtab)
{
	struct Token *operand = find_operand(lexer);
	// implied operand or accumulator
	if (!operand)
		return PARSER_SUCCESS;

	if (operand->type == TOKEN_LABEL) {
		return parse_label_operand(operand, instr, symtab);
	} else if (operand->type == TOKEN_LITERAL) {
		// no need to do anything special to parse literal
		// the lexer did everything necessary
		return PARSER_SUCCESS;
	}
	return ERROR_UNKNOWN;
}

/* apply_masks()
	@lexer          ptr to Lexer struct
	@curr_field     current working bitfield

	@return         resulting bitfield after masks

	Apply bitmasks to @curr_field to weed out the expressed addressing mode.
	Only bitfields pertaining to registers and indirect modes are applied.

	This function will only work partway. The functions below this one will
	finalize the true result bitfield.
*/
int16_t apply_masks(struct Lexer *lexer, int16_t curr_field)
{
	int has_x = 0;
	int has_y = 0;
	int is_indirect = 0;

	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		if (curr->type == TOKEN_X_REGISTER)
			has_x = 1;
		else if (curr->type == TOKEN_Y_REGISTER)
			has_y = 1;
		else if (curr->type == TOKEN_OPEN_PARENTHESIS)
			is_indirect = 1;
	}

	if (has_x)
		curr_field &= X_REGISTER_FIELD;
	else if (has_y)
		curr_field &= Y_REGISTER_FIELD;
	else
		curr_field &= NOT_REGISTER_FIELD;

	if (is_indirect)
		curr_field &= INDIRECT_FIELD;
	else
		curr_field &= NOT_INDIRECT_FIELD;
	return curr_field;
}

/* parse_forward_reference_addr_mode()
	@lexer          ptr to Lexer struct
	@instr          ptr to Instruction struct

	@return         FORWARD_REFERENCE signal code, used by assembler to
	                delay writing bytes until label is resolved

	Saves the information needed to parse a forward reference address when
	we try to resolve the label later. Save info now because we do not save
	all the source lines so we must preserve that info.
*/
int16_t parse_forward_reference_addr_mode(struct Lexer *lexer,
                                          struct Instruction *instr)
{
	int16_t addr_mode = 0x1FFF;
	addr_mode = apply_masks(lexer, addr_mode);

	if (is_branch(instr->mnemonic)) {
		instr->addr_bitflag = addr_mode & ADDR_MODE_RELATIVE;
		return FORWARD_REFERENCE;
	}
	instr->addr_bitflag = addr_mode & ADDR_MODE_ABSOLUTE;
	return FORWARD_REFERENCE;
}

/* parse_addr_mode()
	@operand_status         whether operand is branch/jump and/or forward
	                        reference
	@lexer                  ptr to Lexer struct
	@instr                  ptr to Instruction struct

	@return                 bitmask containing expressed addressing mode

	Apply bit masks to determine the addressing mode of a lexer sequence.
	Any invalid sequences will have incompatible bit masks and an
	instruction bitfield that zero each other out.
*/
int16_t parse_addr_mode(int operand_status, struct Lexer *lexer,
                        struct Instruction *instr)
{
	if (operand_status == BRANCH_FORWARD_REFERENCE ||
	    operand_status == JUMP_FORWARD_REFERENCE)
		return parse_forward_reference_addr_mode(lexer, instr);

	struct Token *operand = find_operand(lexer);
	if (!operand) {
		switch (instr->mnemonic) {
		case ASL:
		case LSR:
		case ROL:
		case ROR:
			return ADDR_MODE_ACCUMULATOR;
		default:
			return ADDR_MODE_IMPLIED;
		}
	}

	int16_t addr_mode = 0x1FFF;
	if (operand->value > 0xFF) {
		addr_mode = ABSOLUTE_FIELD;
	} else {
		// a jump label may have a zero page value but it's still
		// absolute because we always write 3 bytes
		if (!is_jump(instr->mnemonic))
			addr_mode = ZERO_PAGE_FIELD;
		else
			addr_mode = ABSOLUTE_FIELD;
	}

	// branching is always relative
	if (is_branch(instr->mnemonic))
		addr_mode &= ADDR_MODE_RELATIVE;
	else
		addr_mode &= ~ADDR_MODE_RELATIVE;

	// immediate token is either at index 1 or 2
	if (lexer->sequence[1]->type == TOKEN_IMMEDIATE ||
	    lexer->sequence[2]->type == TOKEN_IMMEDIATE)
		addr_mode &= ADDR_MODE_IMMEDIATE;
	else
		addr_mode &= ~ADDR_MODE_IMMEDIATE;

	addr_mode = apply_masks(lexer, addr_mode);
	return addr_mode;
}