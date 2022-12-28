#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdint.h>

int assemble_ADC(int16_t bitflag);
int assemble_AND(int16_t bitflag);
int assemble_ASL(int16_t bitflag);
int assemble_BCC(int16_t bitflag);
int assemble_BCS(int16_t bitflag);
int assemble_BEQ(int16_t bitflag);
int assemble_BIT(int16_t bitflag);
int assemble_BMI(int16_t bitflag);
int assemble_BNE(int16_t bitflag);
int assemble_BPL(int16_t bitflag);
int assemble_BRK(int16_t bitflag);
int assemble_BVC(int16_t bitflag);
int assemble_BVS(int16_t bitflag);
int assemble_CLC(int16_t bitflag);
int assemble_CLD(int16_t bitflag);
int assemble_CLI(int16_t bitflag);
int assemble_CLV(int16_t bitflag);
int assemble_CMP(int16_t bitflag);
int assemble_CPX(int16_t bitflag);
int assemble_CPY(int16_t bitflag);
int assemble_DEC(int16_t bitflag);
int assemble_DEX(int16_t bitflag);
int assemble_DEY(int16_t bitflag);
int assemble_EOR(int16_t bitflag);
int assemble_INC(int16_t bitflag);
int assemble_INX(int16_t bitflag);
int assemble_INY(int16_t bitflag);
int assemble_JMP(int16_t bitflag);
int assemble_JSR(int16_t bitflag);
int assemble_LDA(int16_t bitflag);
int assemble_LDX(int16_t bitflag);
int assemble_LDY(int16_t bitflag);
int assemble_LSR(int16_t bitflag);
int assemble_NOP(int16_t bitflag);
int assemble_ORA(int16_t bitflag);
int assemble_PHA(int16_t bitflag);
int assemble_PHP(int16_t bitflag);
int assemble_PLA(int16_t bitflag);
int assemble_PLP(int16_t bitflag);
int assemble_ROL(int16_t bitflag);
int assemble_ROR(int16_t bitflag);
int assemble_RTI(int16_t bitflag);
int assemble_RTS(int16_t bitflag);
int assemble_SBC(int16_t bitflag);
int assemble_SEC(int16_t bitflag);
int assemble_SED(int16_t bitflag);
int assemble_SEI(int16_t bitflag);
int assemble_STA(int16_t bitflag);
int assemble_STX(int16_t bitflag);
int assemble_STY(int16_t bitflag);
int assemble_TAX(int16_t bitflag);
int assemble_TAY(int16_t bitflag);
int assemble_TSX(int16_t bitflag);
int assemble_TXA(int16_t bitflag);
int assemble_TXS(int16_t bitflag);
int assemble_TYA(int16_t bitflag);

#endif