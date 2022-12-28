/** generator.c

Code generator for the 6502 assembler. Retrieves opcodes and operand bytes, then
writes those bytes to a binary file. The generator also resolves forward
references.

*/

#include <stdio.h>

#include "addressing_modes.h"
#include "lexer.h"
#include "opcode.h"

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

/* resolve_label_ref()
	@f                      ptr to binary FILE
	@instr                  ptr to Instruction struct
	@label                  ptr to token containing the label reference
	@symtab                 symbol table
	@pc                     current program counter location
	@operand_status         whether @label is part of branch/jump
	                        instruction

	@return                 number of bytes written to @f

	Resolve a label reference and then assemble the line. Forward references
	CANNOT be resolved with this function.
*/
// int resolve_label_ref(FILE *f, struct Instruction *instr, struct Token *label,
//                       struct SymbolTable *symtab, int pc, int operand_status)
// {
// 	if (operand_status == BRANCH_OPERAND) {
// 		;
// 	} else if (operand_status == JUMP_OPERAND) {
// 		;
// 	}
// }

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
	int opcode = get_opcode(instr);
	instr->opcode = opcode;
	int operand_bytes = operand->value;

	fseek(f, pc, SEEK_SET);

	fputc(opcode, f);
	if (instr->addr_bitflag & ADDR_MODE_IMPLIED ||
	    instr->addr_bitflag & ADDR_MODE_ACCUMULATOR) {
		return 1;
	} else {
		// little endian
		fputc(operand_bytes & 0xFF, f);
		if (operand_bytes >> 8)
			fputc(operand_bytes >> 8, f);
		return 1 + calc_num_bytes(operand_bytes);
	}
}