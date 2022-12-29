/** generator.c

Code generator for the 6502 assembler. Retrieves opcodes and operand bytes, then
writes those bytes to a binary file. The generator also resolves forward
references.

*/

#include <stdio.h>

#include "addressing_modes.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

/* calc_num_bytes()
	@operand_bytes          value of operand

	@return                 number of bytes in @operand_bytes

	Calculates the number of bytes in an operand's value.
*/
int calc_num_bytes(int operand_bytes)
{
	if (operand_bytes & 0xFF00)
		return 2;
	return 1;
}

/* generate_code()
	@f              ptr to binary FILE
	@instr          ptr to Instruction struct
	@operand        ptr to operand token in a Lexer sequence
	@pc             current program counter location

	@return         number of bytes written to @f

	Assemble a line of code and write the bytes to a binary file.
*/
int generate_code(FILE *f, struct Instruction *instr, struct Token *operand,
                  int pc)
{
	unsigned char opcode = get_opcode(instr);
	instr->opcode = opcode;

	fseek(f, pc, SEEK_SET);
	fputc(opcode, f);

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