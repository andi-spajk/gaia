/** parser.c

Parser for the assembler. Analyze a sequence of tokens prepared by the lexer
to locate syntax errors. Label declarations are inserted into the symbol
table, and forward references and saved. Addresing modes and opcodes are also
determined and saved into the Instruction struct.

The parser determines syntax errors such as illegal sequences of tokens, or
instructions with incompatible addressing modes. Other errors include redefining
label constants, missing label declarations, or illegal forward references.

See parser.h for full diagram of valid token sequences.

*/

#include <stddef.h>

#include "addressing_modes.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

#define IS_DIRECTIVE(type) ((type) == TOKEN_DEFINE_DIRECTIVE || \
                            (type) == TOKEN_ORG_DIRECTIVE ||    \
                            (type) == TOKEN_END_DIRECTIVE)

/* parse_indirect_operand_tree()
	@lexer          ptr to Lexer struct
	@index          index of operand token

	@return         success or error code

	Parse the indirect operand for syntax errors.
*/
int parse_indirect_operand_tree(struct Lexer *lexer, int index)
{
	struct Token **seq = lexer->sequence;
	index++;
	// ,
	if (seq[index]->type == TOKEN_COMMA) {
		index++;
		// ,X
		if (seq[index]->type == TOKEN_X_REGISTER) {
			index++;
			// ,X)
			if (seq[index]->type == TOKEN_CLOSE_PARENTHESIS) {
				index++;
				if (seq[index]->type == TOKEN_NULL)
					return PARSER_SUCCESS;
			}
		}
	}
	// )
	else if (seq[index]->type == TOKEN_CLOSE_PARENTHESIS) {
		index++;
		// ),
		if (seq[index]->type == TOKEN_COMMA) {
			index++;
			// ),Y
			if (seq[index]->type == TOKEN_Y_REGISTER) {
				index++;
				if (seq[index]->type == TOKEN_NULL)
					return PARSER_SUCCESS;
			}
		}
		// )
		else if (seq[index]->type == TOKEN_NULL) {
			return PARSER_SUCCESS;
		}
	}
	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_SEQUENCE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_instr_tree()
	@lexer          ptr to Lexer struct
	@index          index of instruction token

	@return         success or error code

	Parse the instruction token and its operand for syntax errors.
*/
int parse_instr_tree(struct Lexer *lexer, int index)
{
	struct Token **seq = lexer->sequence;
	index++;
	// (
	if (seq[index]->type == TOKEN_OPEN_PARENTHESIS) {
		index++;
		// (OPERAND
		if (seq[index]->type == TOKEN_LABEL ||
		    seq[index]->type == TOKEN_LITERAL)
			return parse_indirect_operand_tree(lexer, index);
	}
	// OPERAND
	else if (seq[index]->type == TOKEN_LABEL ||
	         seq[index]->type == TOKEN_LITERAL) {
		index++;
		if (seq[index]->type == TOKEN_NULL) {
			return PARSER_SUCCESS;
		}
		// OPERAND,
		else if (seq[index]->type == TOKEN_COMMA) {
			index++;
			// OPERAND,X
			// OPERAND,Y
			if (seq[index]->type == TOKEN_X_REGISTER ||
			    seq[index]->type == TOKEN_Y_REGISTER)
				return PARSER_SUCCESS;
		}
	}
	// #
	else if (seq[index]->type == TOKEN_IMMEDIATE) {
		index++;
		// #OPERAND
		// immediate labels are also legal
		if (seq[index]->type == TOKEN_LITERAL ||
		    seq[index]->type == TOKEN_LABEL) {
			index++;
			if (seq[index]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	}
	else if (seq[index]->type == TOKEN_NULL) {
		return PARSER_SUCCESS;
	}
	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_SEQUENCE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_label_tree()
	@lexer          ptr to Lexer struct
	@index          index of label token

	@return         success or error code

	Parse the label line and detect syntax errors.
*/
int parse_label_tree(struct Lexer *lexer, int index)
{
	struct Token **seq = lexer->sequence;
	index++;
	// =
	if (seq[index]->type == TOKEN_EQUAL_SIGN) {
		index++;
		// = $A0
		if (seq[index]->type == TOKEN_LITERAL) {
			index++;
			if (seq[index]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	}
	// LDA
	else if (seq[index]->type == TOKEN_INSTRUCTION) {
		return parse_instr_tree(lexer, index);
	}
	// just a declaration of label
	else if (seq[index]->type == TOKEN_NULL) {
		return PARSER_SUCCESS;
	}
	// .EQU directive
	else if (seq[index]->type == TOKEN_EQU_DIRECTIVE) {
		index++;
		if (seq[index]->type == TOKEN_LITERAL) {
			index++;
			if (seq[index]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	}
	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_SEQUENCE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_directive_tree()
	@lexer          ptr to Lexer struct
	@index          index of directive token

	@return         success or error code

	Parse the directive and detect syntax errors.
*/
int parse_directive_tree(struct Lexer *lexer, int index)
{
	struct Token **seq = lexer->sequence;

	if (seq[index]->type == TOKEN_DEFINE_DIRECTIVE) {
		index++;
		if (seq[index]->type == TOKEN_LABEL) {
			index++;
			if (seq[index]->type == TOKEN_LITERAL) {
				index++;
				if (seq[index]->type == TOKEN_NULL)
					return PARSER_SUCCESS;
			}
		}
	} else if (seq[index]->type == TOKEN_ORG_DIRECTIVE) {
		index++;
		if (seq[index]->type == TOKEN_LITERAL) {
			index++;
			if (seq[index]->type == TOKEN_NULL)
				return PARSER_SUCCESS;
		}
	} else if (seq[index]->type == TOKEN_END_DIRECTIVE) {
		index++;
		if (seq[index]->type == TOKEN_NULL)
			return PARSER_SUCCESS;
	}

	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_SEQUENCE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_base_tree()
	@lexer          ptr to Lexer struct
	@index          index of base token

	@return         success or error code

	Parse the base directive and detect syntax errors.
*/
int parse_base_tree(struct Lexer *lexer, int index)
{
	struct Token **seq = lexer->sequence;
	index++;

	if (seq[index]->type == TOKEN_EQUAL_SIGN) {
		index++;
		if (seq[index]->type == TOKEN_LITERAL) {
			index++;
			if (seq[index]->type == TOKEN_NULL) {
				seq[index-3]->value = seq[index-1]->value;
				return PARSER_SUCCESS;
			}
		}
	}

	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_BASE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_BASE;
}

/* parse_line()
	@lexer          ptr to Lexer struct

	@return         success or error code

	Check @lexer's token sequence for syntax errors.
*/
int parse_line(struct Lexer *lexer)
{
	struct Token **seq = lexer->sequence;
	int index = 0;

	if (seq[index]->type == TOKEN_INSTRUCTION)
		return parse_instr_tree(lexer, index);
	else if (seq[index]->type == TOKEN_LABEL)
		return parse_label_tree(lexer, index);
	else if (IS_DIRECTIVE(seq[index]->type))
		return parse_directive_tree(lexer, index);
	else if (seq[index]->type == TOKEN_BASE)
		return parse_base_tree(lexer, index);
	else if (seq[index]->type == TOKEN_NULL)
		return PARSER_SUCCESS;  // blank or comment line

	lexer->error_tk = lexer->sequence[index];
	print_error(lexer->line, ERROR_ILLEGAL_SEQUENCE,
	            lexer->error_tk->buffer_location, lexer->file_name,
	            lexer->line_num);
	return ERROR_ILLEGAL_SEQUENCE;
}

/* parse_label_declaration()
	@lexer          ptr to Lexer struct
	@symtab         ptr to symbol table
	@pc             current program counter

	@return         success or error code

	Parse a source line that defines a label and insert the label into
	the symbol table. The label's value is either the current program
	counter location, or a constant.

	Label definitions are either lines that begin with a label, or have the
	.DEFINE directive.
*/
int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc)
{
	struct Token **seq = lexer->sequence;
	struct Token *label = seq[0];
	struct Token *define_label = seq[1];
	int found = search_symbol(symtab, label->str);
	int found_define = search_symbol(symtab, define_label->str);

	if (seq[1]->type == TOKEN_NULL || seq[1]->type == TOKEN_INSTRUCTION) {
		// address label
		// might be lone label or a label then instr
		if (found != ERROR_SYMBOL_NOT_FOUND) {
			lexer->error_tk = label;
			print_error(lexer->line, ERROR_LABEL_REDEFINITION,
			            lexer->error_tk->buffer_location,
			            lexer->file_name, lexer->line_num);
			return ERROR_LABEL_REDEFINITION;
		} else {
			label->value = pc;
			return insert_symbol(symtab, label->str, label->value);
		}
	} else if (seq[1]->type == TOKEN_EQUAL_SIGN ||
	           seq[1]->type == TOKEN_EQU_DIRECTIVE) {
		// constant label
		if (found != ERROR_SYMBOL_NOT_FOUND) {
			lexer->error_tk = label;
			print_error(lexer->line, ERROR_LABEL_REDEFINITION,
			            lexer->error_tk->buffer_location,
			            lexer->file_name, lexer->line_num);
			return ERROR_LABEL_REDEFINITION;
		} else {
			// 0     1 2
			// LABEL = $00
			label->value = seq[2]->value;
			return insert_symbol(symtab, label->str, label->value);
		}
	}

	if (seq[0]->type == TOKEN_DEFINE_DIRECTIVE) {
		// label defined by .DEFINE
		if (found_define != ERROR_SYMBOL_NOT_FOUND) {
			lexer->error_tk = define_label;
			print_error(lexer->line, ERROR_LABEL_REDEFINITION,
			            lexer->error_tk->buffer_location,
			            lexer->file_name, lexer->line_num);
			return ERROR_LABEL_REDEFINITION;
		} else {
			define_label->value = seq[2]->value;
			return insert_symbol(symtab, define_label->str,
			                     define_label->value);
		}
	}

	print_error(lexer->line, ERROR_UNKNOWN, NULL, lexer->file_name,
	            lexer->line_num);
	return ERROR_UNKNOWN;
}

/* parse_label_operand()
	@lexer          ptr to Lexer struct
	@instr          ptr to Instruction struct
	@operand        ptr to operand token in a Lexer sequence
	@symtab         ptr to symbol table

	@return         operand status, or error code

	Determine whether the operand is a branch/jump instruction and whether
	it is a forward reference. Operands which are neither are ignored.
	Illegal forward references or label references cause errors.
*/
int parse_label_operand(struct Lexer *lexer, struct Instruction *instr,
                        struct Token *operand, struct SymbolTable *symtab)
{
	int label_value = search_symbol(symtab, operand->str);
	int branch = is_branch(instr->mnemonic);
	int jump = is_jump(instr->mnemonic);

	if (label_value != ERROR_SYMBOL_NOT_FOUND) {
		// save value in both symtab and sequence token
		// allows for faster lookup when we parse the addressing mode
		operand->value = label_value;
		// this assigment means that branch labels may contain an
		// absolute address as its value.
		// this gets fixed in resolve_label_ref() in generator.c
		if (branch)
			return BRANCH_OPERAND;
		else if (jump)
			return JUMP_OPERAND;
		else
			return PARSER_SUCCESS;
	} else {
		// nonexistent symbol may be a forward reference
		// no forward references to constants! (except in indirect jump
		// instructions)
		if (branch) {
			return BRANCH_FORWARD_REFERENCE;
		} else if (jump) {
			return JUMP_FORWARD_REFERENCE;
		} else {
			lexer->error_tk = operand;
			print_error(lexer->line,
			            ERROR_ILLEGAL_FORWARD_REFERENCE,
			            lexer->error_tk->buffer_location,
			            lexer->file_name, lexer->line_num);
			return ERROR_ILLEGAL_FORWARD_REFERENCE;
		}
	}
	print_error(lexer->line, ERROR_UNKNOWN, NULL, lexer->file_name,
	            lexer->line_num);
	return ERROR_UNKNOWN;
}

/* parse_operand()
	@lexer          ptr to Lexer struct
	@instr          ptr to Instruction struct
	@operand        ptr to operand token (can be NULL)
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
                  struct Token *operand, struct SymbolTable *symtab)
{
	// implied operand or accumulator
	if (!operand)
		return PARSER_SUCCESS;

	if (operand->type == TOKEN_LABEL) {
		return parse_label_operand(lexer, instr, operand, symtab);
	} else if (operand->type == TOKEN_LITERAL) {
		// no need to do anything special to parse literal
		// the lexer did everything necessary
		return PARSER_SUCCESS;
	}
	print_error(lexer->line, ERROR_UNKNOWN, NULL, lexer->file_name,
	            lexer->line_num);
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
	if (!lexer)
		return NULL;

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

/* apply_masks()
	@lexer          ptr to Lexer struct
	@curr_field     current working bitfield

	@return         resulting bitfield after masks

	Apply bitmasks to @curr_field to weed out the expressed addressing mode.
	Only bitfields pertaining to registers and indirect modes are applied.

	This function will only work partway. The functions below this one will
	finalize the true result bitfield.
*/
int apply_masks(struct Lexer *lexer, int curr_field)
{
	int has_x = 0;
	int has_y = 0;
	int is_indirect = 0;
	int is_immediate = 0;

	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		if (curr->type == TOKEN_X_REGISTER)
			has_x = 1;
		else if (curr->type == TOKEN_Y_REGISTER)
			has_y = 1;
		else if (curr->type == TOKEN_OPEN_PARENTHESIS)
			is_indirect = 1;
		else if (curr->type == TOKEN_IMMEDIATE)
			is_immediate = 1;
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

	if (is_immediate)
		curr_field &= ADDR_MODE_IMMEDIATE;
	else
		curr_field &= ~ADDR_MODE_IMMEDIATE;
	return curr_field;
}

/* parse_forward_ref_addr_mode()
	@lexer          ptr to Lexer struct
	@instr          ptr to Instruction struct

	@return         FORWARD_REFERENCE success code, used by assembler to
	                delay writing bytes until label is resolved, or error
	                code

	Apply bit masks and specialized checks to parse the addressing mode of a
	forward reference. Saves this information in the instruction struct so
	we can resolve the label later.
*/
int parse_forward_ref_addr_mode(struct Lexer *lexer, struct Instruction *instr)
{
	int addr_mode = 0x1FFF;
	addr_mode = apply_masks(lexer, addr_mode);

	if (is_branch(instr->mnemonic)) {
		instr->addr_bitflag = addr_mode & ADDR_MODE_RELATIVE;
	} else if (instr->mnemonic == JMP) {
		// JMP has two modes, so we AND with the FIELD!!!
		instr->addr_bitflag = addr_mode & ABSOLUTE_FIELD;
	} else if (instr->mnemonic == JSR) {
		// JSR has only one mode
		instr->addr_bitflag = addr_mode & ADDR_MODE_ABSOLUTE;
	}

	if (!instr->addr_bitflag) {
		print_error(lexer->line, ERROR_ILLEGAL_ADDRESSING_MODE, NULL,
		            lexer->file_name, lexer->line_num);
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
	return FORWARD_REFERENCE;
}

/* parse_addr_mode()
	@lexer                  ptr to Lexer struct
	@instr                  ptr to Instruction struct
	@operand                ptr to operand token (can be NULL)
	@operand_status         whether operand is branch/jump and/or forward
	                        reference

	@return                 bitflag containing the expressed addressing
	                        mode, or the FORWARD_REFERENCE success code, or
	                        error code

	Apply bit masks to determine the addressing mode of a lexer sequence.
	@instr will be updated with the new addressing bitflag. Any invalid
	sequences will have an addressing bit mask and an instruction bitfield
	that zero each other out, resulting in a zero bitflag.

	FOR FULL DETAILED EXPLANATION, SEE /src/addressing_modes.h
*/
int parse_addr_mode(struct Lexer *lexer, struct Instruction *instr,
                    struct Token *operand, int operand_status)
{
	if (operand_status == BRANCH_FORWARD_REFERENCE ||
	    operand_status == JUMP_FORWARD_REFERENCE)
		return parse_forward_ref_addr_mode(lexer, instr);

	if (!operand) {
		switch (instr->mnemonic) {
		case ASL:
		case LSR:
		case ROL:
		case ROR:
			instr->addr_bitflag = instr->addr_bitfield &
			                      ADDR_MODE_ACCUMULATOR;
			break;
		default:
			instr->addr_bitflag = instr->addr_bitfield &
			                      ADDR_MODE_IMPLIED;
			break;
		}
		if (!instr->addr_bitflag) {
			print_error(lexer->line, ERROR_ILLEGAL_ADDRESSING_MODE,
			            NULL, lexer->file_name, lexer->line_num);
			return ERROR_ILLEGAL_ADDRESSING_MODE;
		}
		return instr->addr_bitflag;
	}

	int addr_mode = 0x1FFF;
	if (operand->value > 0xFF) {
		// see comment above in parse_label_operand()
		// operand value might be an absolute address, but a branch
		// instruction always has an operand that fits in 1 byte
		if (!is_branch(instr->mnemonic))
			addr_mode = ABSOLUTE_FIELD;
		else
			addr_mode = ZERO_PAGE_FIELD;
	} else {
		// a jump label may have a value that fits in 1 byte, but it's
		// still absolute because we always write 3 bytes for jumps
		if (!is_jump(instr->mnemonic))
			addr_mode = ZERO_PAGE_FIELD;
		else
			addr_mode = ABSOLUTE_FIELD;
	}

	// branching is the only instruction with relative addressing mode
	if (is_branch(instr->mnemonic))
		addr_mode &= ADDR_MODE_RELATIVE;
	else
		addr_mode &= ~ADDR_MODE_RELATIVE;
	// JSR is always absolute
	if (instr->mnemonic == JSR)
		addr_mode &= ADDR_MODE_ABSOLUTE;

	addr_mode = apply_masks(lexer, addr_mode);
	instr->addr_bitflag = addr_mode & instr->addr_bitfield;
	if (!instr->addr_bitflag) {
		print_error(lexer->line, ERROR_ILLEGAL_ADDRESSING_MODE, NULL,
		            lexer->file_name, lexer->line_num);
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
	return instr->addr_bitflag;
}