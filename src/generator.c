/** generator.c

Code generator for the 6502 assembler. Retrieves opcodes and operand bytes, then
writes those bytes to a binary file. The generator also resolves forward
references.

*/

#include <stdio.h>

#include "addressing_modes.h"
#include "error.h"
#include "forward_reference.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

// return statements will be more descriptive
#define ONE_BYTE 1
#define TWO_BYTES 2
#define THREE_BYTES 3

/* generate_code()
	@f              ptr to binary FILE
	@instr          ptr to Instruction struct
	@operand        ptr to operand token (can be NULL)
	@curr_pc        current program counter location

	@return         number of bytes written to @f

	Assemble a line of code and write the bytes to a binary file.
*/
int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int curr_pc)
{
	instr->opcode = get_opcode(instr);

	fseek(f, curr_pc, SEEK_SET);
	fputc(instr->opcode, f);

	int operand_bytes;
	// nonexistent operand indicates accumulator or implied addr mode
	if (operand)
		operand_bytes = operand->value;
	else
		return ONE_BYTE;

	// little endian
	fputc(operand_bytes & 0xFF, f);
	if (instr->addr_bitflag & ABSOLUTE_FIELD) {
		fputc(operand_bytes >> 8, f);
		return THREE_BYTES;
	}
	return TWO_BYTES;
}

/* calc_branch_offset()
	@curr_pc        current program counter location
	@dest_pc        location of destination opcode

	@return         assembled byte for binary file, or error code

	Calculates the actual value of the offset of a branch instruction.
*/
int calc_branch_offset(int curr_pc, int dest_pc)
{
	// offset must be within [-128, 127] or [0x80, 0x7F]
	int offset = dest_pc - curr_pc - 2;
	if (dest_pc <= curr_pc) {
		// branch backward or to same instr
		if (offset < -128)
			return ERROR_TOO_BIG_OFFSET;
	} else {
		// branch forward
		if (offset > 127)
			return ERROR_TOO_BIG_OFFSET;
	}
	return 0xFF & offset;
}

/* resolve_label_ref()
	@f                      ptr to binary FILE
	@lexer          ptr to Lexer struct
	@instr                  ptr to Instruction struct
	@label                  ptr to token containing the label reference
	@operand_status         whether @label is part of branch/jump
	                        instruction
	@symtab                 symbol table
	@curr_pc                current program counter location

	@return                 number of bytes written to @f, or error code

	Resolve a label reference and then assemble the line. Forward references
	CANNOT be resolved with this function.
*/
int resolve_label_ref(FILE *f, struct Lexer *lexer, struct Instruction *instr,
                      struct Token *label, int operand_status,
                      struct SymbolTable *symtab, int curr_pc)
{
	int dest_pc, offset;
	if (label)
		dest_pc = search_symbol(symtab, label->str);

	if (operand_status == BRANCH_OPERAND) {
		offset = calc_branch_offset(curr_pc, dest_pc);
		if (offset == ERROR_TOO_BIG_OFFSET) {
			print_error(lexer->line, ERROR_TOO_BIG_OFFSET,
			            label->buffer_location, lexer->file_name,
			            lexer->line_num);
			return ERROR_TOO_BIG_OFFSET;
		}
		label->value = (unsigned int)offset;
		return generate_code(f, instr, label, curr_pc);
	} else if (operand_status == JUMP_OPERAND) {
		label->value = dest_pc;
		return generate_code(f, instr, label, curr_pc);
	}
	print_error(lexer->line, ERROR_UNKNOWN, NULL, lexer->file_name,
	            lexer->line_num);
	return ERROR_UNKNOWN;
}

/* resolve_forward_ref()
	@f              ptr to binary FILE
	@ref            ptr to ForwardRef struct
	@lexer          ptr to Lexer struct
	@symtab         symbol table

	@return         number of bytes written to @f, or error code

	Resolve a forward reference and assemble the code.

	This function expects a valid forward reference, ie it is the caller's
	responsibility to check if the label had been defined in the midst of
	normal assembly.
*/
int resolve_forward_ref(FILE *f, struct ForwardRef *ref, struct Lexer *lexer,
                        struct SymbolTable *symtab)
{
	struct Instruction *instr = ref->instr;
	int dest_pc, offset;
	dest_pc = search_symbol(symtab, ref->label);

	if (ref->operand_status == BRANCH_FORWARD_REFERENCE) {
		offset = calc_branch_offset(ref->pc, dest_pc);
		if (offset == ERROR_TOO_BIG_OFFSET) {
			print_error(ref->source_line, ERROR_TOO_BIG_OFFSET,
			            ref->operand_location, lexer->file_name,
			            ref->line_num);
			return ERROR_TOO_BIG_OFFSET;
		}

		if (instr->addr_bitflag) {
			instr->opcode = get_opcode(instr);
			fseek(f, ref->pc, SEEK_SET);
			fputc(instr->opcode, f);
			fputc(offset & 0xFF, f);
			return TWO_BYTES;
		}
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	} else if (ref->operand_status == JUMP_FORWARD_REFERENCE) {
		if (instr->addr_bitflag) {
			instr->opcode = get_opcode(instr);
			fseek(f, ref->pc, SEEK_SET);
			fputc(instr->opcode, f);
			fputc(dest_pc & 0xFF, f);
			fputc(dest_pc >> 8, f);
			return THREE_BYTES;
		}
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
	print_error(lexer->line, ERROR_UNKNOWN, NULL, lexer->file_name,
	            lexer->line_num);
	return ERROR_UNKNOWN;
}