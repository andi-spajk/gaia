/** assemble.c

Assemble the opcode for each mnemonic, given an Instruction bitflag. An error
code is returned if the bitflag does not denote any valid addressing mode, ie
the instruction and the addressing mode were not compatible.

Auto-generated with /tests/assemble/gen/gen_assemble.py

*/

#include "addressing_modes.h"
#include "error.h"

int assemble_ADC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0x69;
	case ADDR_MODE_ZERO_PAGE: return 0x65;
	case ADDR_MODE_ZERO_PAGE_X: return 0x75;
	case ADDR_MODE_ABSOLUTE: return 0x6d;
	case ADDR_MODE_ABSOLUTE_X: return 0x7d;
	case ADDR_MODE_ABSOLUTE_Y: return 0x79;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0x61;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0x71;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_AND(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0x29;
	case ADDR_MODE_ZERO_PAGE: return 0x25;
	case ADDR_MODE_ZERO_PAGE_X: return 0x35;
	case ADDR_MODE_ABSOLUTE: return 0x2d;
	case ADDR_MODE_ABSOLUTE_X: return 0x3d;
	case ADDR_MODE_ABSOLUTE_Y: return 0x39;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0x21;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0x31;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_ASL(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ACCUMULATOR: return 0xa;
	case ADDR_MODE_ZERO_PAGE: return 0x6;
	case ADDR_MODE_ZERO_PAGE_X: return 0x16;
	case ADDR_MODE_ABSOLUTE: return 0xe;
	case ADDR_MODE_ABSOLUTE_X: return 0x1e;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BCC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0x90;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BCS(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0xb0;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BEQ(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0xf0;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BIT(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0x24;
	case ADDR_MODE_ABSOLUTE: return 0x2c;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BMI(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0x30;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BNE(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0xd0;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BPL(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0x10;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BRK(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x0;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BVC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0x50;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_BVS(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_RELATIVE: return 0x70;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CLC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x18;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CLD(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xd8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CLI(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x58;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CLV(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xb8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CMP(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xc9;
	case ADDR_MODE_ZERO_PAGE: return 0xc5;
	case ADDR_MODE_ZERO_PAGE_X: return 0xd5;
	case ADDR_MODE_ABSOLUTE: return 0xcd;
	case ADDR_MODE_ABSOLUTE_X: return 0xdd;
	case ADDR_MODE_ABSOLUTE_Y: return 0xd9;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0xc1;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0xd1;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CPX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xe0;
	case ADDR_MODE_ZERO_PAGE: return 0xe4;
	case ADDR_MODE_ABSOLUTE: return 0xec;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_CPY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xc0;
	case ADDR_MODE_ZERO_PAGE: return 0xc4;
	case ADDR_MODE_ABSOLUTE: return 0xcc;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_DEC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0xc6;
	case ADDR_MODE_ZERO_PAGE_X: return 0xd6;
	case ADDR_MODE_ABSOLUTE: return 0xce;
	case ADDR_MODE_ABSOLUTE_X: return 0xde;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_DEX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xca;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_DEY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x88;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_EOR(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0x49;
	case ADDR_MODE_ZERO_PAGE: return 0x45;
	case ADDR_MODE_ZERO_PAGE_X: return 0x55;
	case ADDR_MODE_ABSOLUTE: return 0x4d;
	case ADDR_MODE_ABSOLUTE_X: return 0x5d;
	case ADDR_MODE_ABSOLUTE_Y: return 0x59;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0x41;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0x51;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_INC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0xe6;
	case ADDR_MODE_ZERO_PAGE_X: return 0xf6;
	case ADDR_MODE_ABSOLUTE: return 0xee;
	case ADDR_MODE_ABSOLUTE_X: return 0xfe;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_INX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xe8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_INY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xc8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_JMP(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ABSOLUTE: return 0x4c;
	case ADDR_MODE_INDIRECT: return 0x6c;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_JSR(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ABSOLUTE: return 0x20;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_LDA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xa9;
	case ADDR_MODE_ZERO_PAGE: return 0xa5;
	case ADDR_MODE_ZERO_PAGE_X: return 0xb5;
	case ADDR_MODE_ABSOLUTE: return 0xad;
	case ADDR_MODE_ABSOLUTE_X: return 0xbd;
	case ADDR_MODE_ABSOLUTE_Y: return 0xb9;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0xa1;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0xb1;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_LDX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xa2;
	case ADDR_MODE_ZERO_PAGE: return 0xa6;
	case ADDR_MODE_ZERO_PAGE_Y: return 0xb6;
	case ADDR_MODE_ABSOLUTE: return 0xae;
	case ADDR_MODE_ABSOLUTE_Y: return 0xbe;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_LDY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xa0;
	case ADDR_MODE_ZERO_PAGE: return 0xa4;
	case ADDR_MODE_ZERO_PAGE_X: return 0xb4;
	case ADDR_MODE_ABSOLUTE: return 0xac;
	case ADDR_MODE_ABSOLUTE_X: return 0xbc;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_LSR(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ACCUMULATOR: return 0x4a;
	case ADDR_MODE_ZERO_PAGE: return 0x46;
	case ADDR_MODE_ZERO_PAGE_X: return 0x56;
	case ADDR_MODE_ABSOLUTE: return 0x4e;
	case ADDR_MODE_ABSOLUTE_X: return 0x5e;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_NOP(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xea;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_ORA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0x9;
	case ADDR_MODE_ZERO_PAGE: return 0x5;
	case ADDR_MODE_ZERO_PAGE_X: return 0x15;
	case ADDR_MODE_ABSOLUTE: return 0xd;
	case ADDR_MODE_ABSOLUTE_X: return 0x1d;
	case ADDR_MODE_ABSOLUTE_Y: return 0x19;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0x1;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0x11;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_PHA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x48;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_PHP(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_PLA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x68;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_PLP(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x28;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_ROL(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ACCUMULATOR: return 0x2a;
	case ADDR_MODE_ZERO_PAGE: return 0x26;
	case ADDR_MODE_ZERO_PAGE_X: return 0x36;
	case ADDR_MODE_ABSOLUTE: return 0x2e;
	case ADDR_MODE_ABSOLUTE_X: return 0x3e;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_ROR(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ACCUMULATOR: return 0x6a;
	case ADDR_MODE_ZERO_PAGE: return 0x66;
	case ADDR_MODE_ZERO_PAGE_X: return 0x76;
	case ADDR_MODE_ABSOLUTE: return 0x7e;
	case ADDR_MODE_ABSOLUTE_X: return 0x6e;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_RTI(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x40;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_RTS(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x60;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_SBC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMMEDIATE: return 0xe9;
	case ADDR_MODE_ZERO_PAGE: return 0xe5;
	case ADDR_MODE_ZERO_PAGE_X: return 0xf5;
	case ADDR_MODE_ABSOLUTE: return 0xed;
	case ADDR_MODE_ABSOLUTE_X: return 0xfd;
	case ADDR_MODE_ABSOLUTE_Y: return 0xf9;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0xe1;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0xf1;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_SEC(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x38;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_SED(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xf8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_SEI(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x78;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_STA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0x85;
	case ADDR_MODE_ZERO_PAGE_X: return 0x95;
	case ADDR_MODE_ABSOLUTE: return 0x8d;
	case ADDR_MODE_ABSOLUTE_X: return 0x9d;
	case ADDR_MODE_ABSOLUTE_Y: return 0x99;
	case ADDR_MODE_X_INDEXED_INDIRECT: return 0x81;
	case ADDR_MODE_INDIRECT_Y_INDEXED: return 0x91;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_STX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0x86;
	case ADDR_MODE_ZERO_PAGE_Y: return 0x96;
	case ADDR_MODE_ABSOLUTE: return 0x8e;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_STY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_ZERO_PAGE: return 0x84;
	case ADDR_MODE_ZERO_PAGE_X: return 0x94;
	case ADDR_MODE_ABSOLUTE: return 0x8c;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TAX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xaa;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TAY(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xa8;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TSX(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0xba;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TXA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x8a;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TXS(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x9a;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

int assemble_TYA(int bitflag)
{
	switch (bitflag) {
	case ADDR_MODE_IMPLIED: return 0x98;
	default: return ERROR_ILLEGAL_ADDRESSING_MODE;
	}
}

