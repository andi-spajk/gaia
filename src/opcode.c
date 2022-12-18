/** opcode.c

Functions for initializing instruction structs. Process those instructions to
retrieve opcodes, addressing bit flags/fields, and mnemonics.

*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bitfields.h"
#include "error.h"
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

	instr->mnemonic = NULL_MNEMONIC;
	instr->addr_bitfield = 0;
	instr->addr_bitflag = 0;
	instr->opcode = 0;
	return instr;
}

/* reset_instruction()
	@instr          ptr to Instruction struct

	Resets the members of the Instruction to original initialized values.
	Used by the lexical analyzer to prepare for the next source line.
*/
void reset_instruction(struct Instruction *instr)
{
	instr->mnemonic = NULL_MNEMONIC;
	instr->addr_bitfield = 0;
	instr->addr_bitflag = 0;
	instr->opcode = 0;
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

int is_branch(const enum Mnemonic mnemonic)
{
	switch (mnemonic) {
	case BCC:
	case BCS:
	case BEQ:
	case BMI:
	case BNE:
	case BPL:
	case BVC:
	case BVS:
		return 1;
	default:
		return 0;
	}
}

int is_jump(const enum Mnemonic mnemonic)
{
	return mnemonic == JMP || mnemonic == JSR;
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
enum Mnemonic str_to_mnemonic(const char *str)
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

/* get_addr_bitfield()
	@mnemonic       enum of an instruction mnemonic

	@return         bitfield of valid addressing modes

	Determines a mnemonic's corresponding bitfield which represents the
	valid addressing modes of that mnemonic
*/
int16_t get_addr_bitfield(const enum Mnemonic mnemonic)
{
	switch (mnemonic) {
	case ADC: return ADC_BITFIELD;
	case AND: return AND_BITFIELD;
	case ASL: return ASL_BITFIELD;
	case BCC: return BCC_BITFIELD;
	case BCS: return BCS_BITFIELD;
	case BEQ: return BEQ_BITFIELD;
	case BIT: return BIT_BITFIELD;
	case BMI: return BMI_BITFIELD;
	case BNE: return BNE_BITFIELD;
	case BPL: return BPL_BITFIELD;
	case BRK: return BRK_BITFIELD;
	case BVC: return BVC_BITFIELD;
	case BVS: return BVS_BITFIELD;
	case CLC: return CLC_BITFIELD;
	case CLD: return CLD_BITFIELD;
	case CLI: return CLI_BITFIELD;
	case CLV: return CLV_BITFIELD;
	case CMP: return CMP_BITFIELD;
	case CPX: return CPX_BITFIELD;
	case CPY: return CPY_BITFIELD;
	case DEC: return DEC_BITFIELD;
	case DEX: return DEX_BITFIELD;
	case DEY: return DEY_BITFIELD;
	case EOR: return EOR_BITFIELD;
	case INC: return INC_BITFIELD;
	case INX: return INX_BITFIELD;
	case INY: return INY_BITFIELD;
	case JMP: return JMP_BITFIELD;
	case JSR: return JSR_BITFIELD;
	case LDA: return LDA_BITFIELD;
	case LDX: return LDX_BITFIELD;
	case LDY: return LDY_BITFIELD;
	case LSR: return LSR_BITFIELD;
	case NOP: return NOP_BITFIELD;
	case ORA: return ORA_BITFIELD;
	case PHA: return PHA_BITFIELD;
	case PHP: return PHP_BITFIELD;
	case PLA: return PLA_BITFIELD;
	case PLP: return PLP_BITFIELD;
	case ROL: return ROL_BITFIELD;
	case ROR: return ROR_BITFIELD;
	case RTI: return RTI_BITFIELD;
	case RTS: return RTS_BITFIELD;
	case SBC: return SBC_BITFIELD;
	case SEC: return SEC_BITFIELD;
	case SED: return SED_BITFIELD;
	case SEI: return SEI_BITFIELD;
	case STA: return STA_BITFIELD;
	case STX: return STX_BITFIELD;
	case STY: return STY_BITFIELD;
	case TAX: return TAX_BITFIELD;
	case TAY: return TAY_BITFIELD;
	case TSX: return TSX_BITFIELD;
	case TXA: return TXA_BITFIELD;
	case TXS: return TXS_BITFIELD;
	case TYA: return TYA_BITFIELD;
	default: return ERROR_BITFIELD_NOT_FOUND;
	}
}