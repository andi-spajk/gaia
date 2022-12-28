/** opcode.c

Functions for initializing instruction structs. Process those instructions to
retrieve opcodes, addressing bit flags/fields, and mnemonics.

*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "assemble.h"
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

/* is_branch()
	@mnemonic       mnemonic to check

	@return         1 if branch mnemonic, 0 if not

	Check if an instruction mnemonic is a branch instruction.
*/
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

/* is_jump()
	@mnemonic       mnemonic to check

	@return         1 if jump mnemonic, 0 if not

	Check if an instruction mnemonic is a jump instruction.
*/
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

int get_opcode(struct Instruction *instr)
{
	switch (instr->mnemonic) {
	case ADC: return assemble_ADC(instr->addr_bitflag);
	case AND: return assemble_AND(instr->addr_bitflag);
	case ASL: return assemble_ASL(instr->addr_bitflag);
	case BCC: return assemble_BCC(instr->addr_bitflag);
	case BCS: return assemble_BCS(instr->addr_bitflag);
	case BEQ: return assemble_BEQ(instr->addr_bitflag);
	case BIT: return assemble_BIT(instr->addr_bitflag);
	case BMI: return assemble_BMI(instr->addr_bitflag);
	case BNE: return assemble_BNE(instr->addr_bitflag);
	case BPL: return assemble_BPL(instr->addr_bitflag);
	case BRK: return assemble_BRK(instr->addr_bitflag);
	case BVC: return assemble_BVC(instr->addr_bitflag);
	case BVS: return assemble_BVS(instr->addr_bitflag);
	case CLC: return assemble_CLC(instr->addr_bitflag);
	case CLD: return assemble_CLD(instr->addr_bitflag);
	case CLI: return assemble_CLI(instr->addr_bitflag);
	case CLV: return assemble_CLV(instr->addr_bitflag);
	case CMP: return assemble_CMP(instr->addr_bitflag);
	case CPX: return assemble_CPX(instr->addr_bitflag);
	case CPY: return assemble_CPY(instr->addr_bitflag);
	case DEC: return assemble_DEC(instr->addr_bitflag);
	case DEX: return assemble_DEX(instr->addr_bitflag);
	case DEY: return assemble_DEY(instr->addr_bitflag);
	case EOR: return assemble_EOR(instr->addr_bitflag);
	case INC: return assemble_INC(instr->addr_bitflag);
	case INX: return assemble_INX(instr->addr_bitflag);
	case INY: return assemble_INY(instr->addr_bitflag);
	case JMP: return assemble_JMP(instr->addr_bitflag);
	case JSR: return assemble_JSR(instr->addr_bitflag);
	case LDA: return assemble_LDA(instr->addr_bitflag);
	case LDX: return assemble_LDX(instr->addr_bitflag);
	case LDY: return assemble_LDY(instr->addr_bitflag);
	case LSR: return assemble_LSR(instr->addr_bitflag);
	case NOP: return assemble_NOP(instr->addr_bitflag);
	case ORA: return assemble_ORA(instr->addr_bitflag);
	case PHA: return assemble_PHA(instr->addr_bitflag);
	case PHP: return assemble_PHP(instr->addr_bitflag);
	case PLA: return assemble_PLA(instr->addr_bitflag);
	case PLP: return assemble_PLP(instr->addr_bitflag);
	case ROL: return assemble_ROL(instr->addr_bitflag);
	case ROR: return assemble_ROR(instr->addr_bitflag);
	case RTI: return assemble_RTI(instr->addr_bitflag);
	case RTS: return assemble_RTS(instr->addr_bitflag);
	case SBC: return assemble_SBC(instr->addr_bitflag);
	case SEC: return assemble_SEC(instr->addr_bitflag);
	case SED: return assemble_SED(instr->addr_bitflag);
	case SEI: return assemble_SEI(instr->addr_bitflag);
	case STA: return assemble_STA(instr->addr_bitflag);
	case STX: return assemble_STX(instr->addr_bitflag);
	case STY: return assemble_STY(instr->addr_bitflag);
	case TAX: return assemble_TAX(instr->addr_bitflag);
	case TAY: return assemble_TAY(instr->addr_bitflag);
	case TSX: return assemble_TSX(instr->addr_bitflag);
	case TXA: return assemble_TXA(instr->addr_bitflag);
	case TXS: return assemble_TXS(instr->addr_bitflag);
	case TYA: return assemble_TYA(instr->addr_bitflag);
	default: return ERROR_ILLEGAL_MNEMONIC;
	}
}