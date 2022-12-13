/** opcode.c

Functions for initializing instruction structs. Process those instructions to
retrieve opcodes, addressing bit flags/fields, and mnemonics.

*/

#include <stdlib.h>
#include <string.h>

#include "opcode.h"

/* init_instruction()
	@return         ptr to dynamically allocated Instruction struct, or
	                NULL if fail

	Dynamically allocates an Instruction struct and initializes its
	members.
*/
struct Instruction *init_instruction(void)
{
	struct Instruction *instr = malloc(sizeof(struct Instruction));
	if (!instr)
		return NULL;

	instr->mnemonic = ILLEGAL_MNEMONIC;
	instr->addr_field = 0;
	instr->addr_flag = 0;
	instr->opcode = 0;
	return instr;
}

/* destroy_instruction()
	@instr          ptr to Instruction struct

	Frees the memory used by a dynamically allocated Instruction struct.
*/
void destroy_instruction(struct Instruction *instr)
{
	if (instr)
		free(instr);
}

/* str_to_mnemonic()
	@str            string containing possible mnemonic

	@return         corresponding mnemonic enum of the string, or
	                ILLEGAL_MNEMONIC if nonexistent

	Reads a string representation of an instruction mnemonic and determines
	its corresponding enum mnemonic.

	THE LEXER SHOULD GUARANTEE THAT ONLY ALL-UPPERCASE STRINGS ARE PASSED
	INTO THIS FUNCTION.
*/
enum Mnemonic str_to_mnemonic(char *str)
{
	if (!strcmp("ADC", str)) return ADC;
	else if (!strcmp("AND", str)) return AND;
	else if (!strcmp("ASL", str)) return ASL;
	else if (!strcmp("BCC", str)) return BCC;
	else if (!strcmp("BCS", str)) return BCS;
	else if (!strcmp("BEQ", str)) return BEQ;
	else if (!strcmp("BIT", str)) return BIT;
	else if (!strcmp("BMI", str)) return BMI;
	else if (!strcmp("BNE", str)) return BNE;
	else if (!strcmp("BPL", str)) return BPL;
	else if (!strcmp("BRK", str)) return BRK;
	else if (!strcmp("BVC", str)) return BVC;
	else if (!strcmp("BVS", str)) return BVS;
	else if (!strcmp("CLC", str)) return CLC;
	else if (!strcmp("CLD", str)) return CLD;
	else if (!strcmp("CLI", str)) return CLI;
	else if (!strcmp("CLV", str)) return CLV;
	else if (!strcmp("CMP", str)) return CMP;
	else if (!strcmp("CPX", str)) return CPX;
	else if (!strcmp("CPY", str)) return CPY;
	else if (!strcmp("DEC", str)) return DEC;
	else if (!strcmp("DEX", str)) return DEX;
	else if (!strcmp("DEY", str)) return DEY;
	else if (!strcmp("EOR", str)) return EOR;
	else if (!strcmp("INC", str)) return INC;
	else if (!strcmp("INX", str)) return INX;
	else if (!strcmp("INY", str)) return INY;
	else if (!strcmp("JMP", str)) return JMP;
	else if (!strcmp("JSR", str)) return JSR;
	else if (!strcmp("LDA", str)) return LDA;
	else if (!strcmp("LDX", str)) return LDX;
	else if (!strcmp("LDY", str)) return LDY;
	else if (!strcmp("LSR", str)) return LSR;
	else if (!strcmp("NOP", str)) return NOP;
	else if (!strcmp("ORA", str)) return ORA;
	else if (!strcmp("PHA", str)) return PHA;
	else if (!strcmp("PHP", str)) return PHP;
	else if (!strcmp("PLA", str)) return PLA;
	else if (!strcmp("PLP", str)) return PLP;
	else if (!strcmp("ROL", str)) return ROL;
	else if (!strcmp("ROR", str)) return ROR;
	else if (!strcmp("RTI", str)) return RTI;
	else if (!strcmp("RTS", str)) return RTS;
	else if (!strcmp("SBC", str)) return SBC;
	else if (!strcmp("SEC", str)) return SEC;
	else if (!strcmp("SED", str)) return SED;
	else if (!strcmp("SEI", str)) return SEI;
	else if (!strcmp("STA", str)) return STA;
	else if (!strcmp("STX", str)) return STX;
	else if (!strcmp("STY", str)) return STY;
	else if (!strcmp("TAX", str)) return TAX;
	else if (!strcmp("TAY", str)) return TAY;
	else if (!strcmp("TSX", str)) return TSX;
	else if (!strcmp("TXA", str)) return TXA;
	else if (!strcmp("TXS", str)) return TXS;
	else if (!strcmp("TYA", str)) return TYA;
	return ILLEGAL_MNEMONIC;
}