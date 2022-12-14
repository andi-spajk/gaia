#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

enum Mnemonic {
	ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI,
	BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI,
	CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR,
	INC, INX, INY, JMP, JSR, LDA, LDX, LDY,
	LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL,
	ROR, RTI, RTS, SBC, SEC, SED, SEI, STA,
	STX, STY, TAX, TAY, TSX, TXA, TXS, TYA,
	NUM_MNEMONICS,
	ILLEGAL_MNEMONIC
};

struct Instruction {
	enum Mnemonic mnemonic;
	int16_t addr_bitfield;
	int16_t addr_bitflag;
	int opcode;
};

struct Instruction *init_instruction(void);
void destroy_instruction(struct Instruction *instr);
enum Mnemonic str_to_mnemonic(char *str);
int16_t get_addr_bitfield(enum Mnemonic mnemonic);

#endif