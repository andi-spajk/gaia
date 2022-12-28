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
		return 3;
	return 2;
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
	int opcode = get_opcode(instr);
	instr->opcode = opcode;
	int operand_bytes = operand->value;

	fseek(f, pc, SEEK_SET);

	fputc(opcode, f);
	if (instr->addr_bitflag != ADDR_MODE_IMPLIED &&
	    instr->addr_bitflag != ADDR_MODE_ACCUMULATOR) {
		// little endian
		fputc(operand_bytes & 0xFF, f);
		if (operand_bytes >> 8)
			fputc(operand_bytes >> 8, f);
		return calc_num_bytes(operand_bytes);
	}
	return 1;
}