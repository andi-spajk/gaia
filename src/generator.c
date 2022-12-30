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

/* generate_code()
	@f              ptr to binary FILE
	@instr          ptr to Instruction struct
	@operand        ptr to operand token in a Lexer sequence, or NULL
	@pc             current program counter location

	@return         number of bytes written to @f

	Assemble a line of code and write the bytes to a binary file.
*/
int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int pc)
{
	instr->opcode = get_opcode(instr);

	fseek(f, pc, SEEK_SET);
	fputc(instr->opcode, f);

	int operand_bytes;
	if (operand)
		operand_bytes = operand->value;
	else
		return 1;

	// little endian
	fputc(operand_bytes & 0xFF, f);
	if (instr->addr_bitflag & ABSOLUTE_FIELD) {
		fputc(operand_bytes >> 8, f);
		return 3;
	}
	return 2;
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
	return 0xFF & (dest_pc - curr_pc - 2);
}

/* resolve_label_ref()
	@f                      ptr to binary FILE
	@instr                  ptr to Instruction struct
	@label                  ptr to token containing the label reference
	@operand_status         whether @label is part of branch/jump
	                        instruction
	@symtab                 symbol table
	@pc                     current program counter location

	@return                 number of bytes written to @f, or error code

	Resolve a label reference and then assemble the line. Forward references
	CANNOT be resolved with this function.
*/
int resolve_label_ref(FILE *f, struct Instruction *instr, struct Token *label,
                      int operand_status, struct SymbolTable *symtab, int pc)
{
	int dest_pc, offset;
	if (label)
		dest_pc = search_symbol(symtab, label->str);

	if (operand_status == BRANCH_OPERAND) {
		offset = calc_branch_offset(pc, dest_pc);
		if (offset == ERROR_TOO_BIG_OFFSET)
			return ERROR_TOO_BIG_OFFSET;
		label->value = (unsigned int)offset;
		return generate_code(f, instr, label, pc);
	} else if (operand_status == JUMP_OPERAND) {
		label->value = dest_pc;
		return generate_code(f, instr, label, pc);
	}
	return ERROR_UNKNOWN;
}

/* resolve_forward_ref()
	@f              ptr to binary FILE
	@instr          ptr to Instruction struct
	@ref            ptr to ForwardRef struct
	@symtab         symbol table
	@pc             current program counter location

	@return         number of bytes written to @f, or error code

	Resolve a forward reference and assemble the code.

	This function expects a valid forward reference, ie it is the caller's
	responsibility to check if the label had been defined in the midst of
	normal assembly.
*/
int resolve_forward_ref(FILE *f, struct ForwardRef *ref,
                        struct SymbolTable *symtab)
{
	struct Instruction *instr = ref->instr;
	int dest_pc, offset;
	if (ref->label)
		dest_pc = search_symbol(symtab, ref->label);

	if (ref->operand_status == BRANCH_FORWARD_REFERENCE) {
		offset = calc_branch_offset(ref->pc, dest_pc);
		if (offset == ERROR_TOO_BIG_OFFSET)
			return ERROR_TOO_BIG_OFFSET;

		if (instr->addr_bitflag) {
			instr->opcode = get_opcode(instr);
			fseek(f, ref->pc, SEEK_SET);
			fputc(instr->opcode, f);
			fputc(offset & 0xFF, f);
			return 2;
		}
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	} else if (ref->operand_status == JUMP_FORWARD_REFERENCE) {
		if (instr->addr_bitflag) {
			instr->opcode = get_opcode(instr);
			fseek(f, ref->pc, SEEK_SET);
			fputc(instr->opcode, f);
			fputc(dest_pc & 0xFF, f);
			fputc(dest_pc >> 8, f);
			return 3;
		}
		return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
	return ERROR_UNKNOWN;
}