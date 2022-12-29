/** assemble.h

Opcode assembly functions. Checks a mnemonic's valid addressing modes against
its bitflag and assembles the corresponding opcode.

Auto-generated with /tests/assemble/gen/gen_assemble.py

*/

#ifndef ASSEMBLE_H
#define ASSEMBLE_H

int assemble_ADC(int bitflag);
int assemble_AND(int bitflag);
int assemble_ASL(int bitflag);
int assemble_BCC(int bitflag);
int assemble_BCS(int bitflag);
int assemble_BEQ(int bitflag);
int assemble_BIT(int bitflag);
int assemble_BMI(int bitflag);
int assemble_BNE(int bitflag);
int assemble_BPL(int bitflag);
int assemble_BRK(int bitflag);
int assemble_BVC(int bitflag);
int assemble_BVS(int bitflag);
int assemble_CLC(int bitflag);
int assemble_CLD(int bitflag);
int assemble_CLI(int bitflag);
int assemble_CLV(int bitflag);
int assemble_CMP(int bitflag);
int assemble_CPX(int bitflag);
int assemble_CPY(int bitflag);
int assemble_DEC(int bitflag);
int assemble_DEX(int bitflag);
int assemble_DEY(int bitflag);
int assemble_EOR(int bitflag);
int assemble_INC(int bitflag);
int assemble_INX(int bitflag);
int assemble_INY(int bitflag);
int assemble_JMP(int bitflag);
int assemble_JSR(int bitflag);
int assemble_LDA(int bitflag);
int assemble_LDX(int bitflag);
int assemble_LDY(int bitflag);
int assemble_LSR(int bitflag);
int assemble_NOP(int bitflag);
int assemble_ORA(int bitflag);
int assemble_PHA(int bitflag);
int assemble_PHP(int bitflag);
int assemble_PLA(int bitflag);
int assemble_PLP(int bitflag);
int assemble_ROL(int bitflag);
int assemble_ROR(int bitflag);
int assemble_RTI(int bitflag);
int assemble_RTS(int bitflag);
int assemble_SBC(int bitflag);
int assemble_SEC(int bitflag);
int assemble_SED(int bitflag);
int assemble_SEI(int bitflag);
int assemble_STA(int bitflag);
int assemble_STX(int bitflag);
int assemble_STY(int bitflag);
int assemble_TAX(int bitflag);
int assemble_TAY(int bitflag);
int assemble_TSX(int bitflag);
int assemble_TXA(int bitflag);
int assemble_TXS(int bitflag);
int assemble_TYA(int bitflag);

#endif