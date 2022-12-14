#include "../../unity/unity.h"

#include "bitfields.h"
#include "error.h"
#include "opcode.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_instr(void)
{
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitfield);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(0, instr->opcode);

	destroy_instruction(instr);
}

void test_str_to_mnemonic(void)
{
	TEST_ASSERT_EQUAL_INT(ADC, str_to_mnemonic("ADC"));
	TEST_ASSERT_EQUAL_INT(AND, str_to_mnemonic("AND"));
	TEST_ASSERT_EQUAL_INT(ASL, str_to_mnemonic("ASL"));
	TEST_ASSERT_EQUAL_INT(BCC, str_to_mnemonic("BCC"));
	TEST_ASSERT_EQUAL_INT(BCS, str_to_mnemonic("BCS"));
	TEST_ASSERT_EQUAL_INT(BEQ, str_to_mnemonic("BEQ"));
	TEST_ASSERT_EQUAL_INT(BIT, str_to_mnemonic("BIT"));
	TEST_ASSERT_EQUAL_INT(BMI, str_to_mnemonic("BMI"));
	TEST_ASSERT_EQUAL_INT(BNE, str_to_mnemonic("BNE"));
	TEST_ASSERT_EQUAL_INT(BPL, str_to_mnemonic("BPL"));
	TEST_ASSERT_EQUAL_INT(BRK, str_to_mnemonic("BRK"));
	TEST_ASSERT_EQUAL_INT(BVC, str_to_mnemonic("BVC"));
	TEST_ASSERT_EQUAL_INT(BVS, str_to_mnemonic("BVS"));
	TEST_ASSERT_EQUAL_INT(CLC, str_to_mnemonic("CLC"));
	TEST_ASSERT_EQUAL_INT(CLD, str_to_mnemonic("CLD"));
	TEST_ASSERT_EQUAL_INT(CLI, str_to_mnemonic("CLI"));
	TEST_ASSERT_EQUAL_INT(CLV, str_to_mnemonic("CLV"));
	TEST_ASSERT_EQUAL_INT(CMP, str_to_mnemonic("CMP"));
	TEST_ASSERT_EQUAL_INT(CPX, str_to_mnemonic("CPX"));
	TEST_ASSERT_EQUAL_INT(CPY, str_to_mnemonic("CPY"));
	TEST_ASSERT_EQUAL_INT(DEC, str_to_mnemonic("DEC"));
	TEST_ASSERT_EQUAL_INT(DEX, str_to_mnemonic("DEX"));
	TEST_ASSERT_EQUAL_INT(DEY, str_to_mnemonic("DEY"));
	TEST_ASSERT_EQUAL_INT(EOR, str_to_mnemonic("EOR"));
	TEST_ASSERT_EQUAL_INT(INC, str_to_mnemonic("INC"));
	TEST_ASSERT_EQUAL_INT(INX, str_to_mnemonic("INX"));
	TEST_ASSERT_EQUAL_INT(INY, str_to_mnemonic("INY"));
	TEST_ASSERT_EQUAL_INT(JMP, str_to_mnemonic("JMP"));
	TEST_ASSERT_EQUAL_INT(JSR, str_to_mnemonic("JSR"));
	TEST_ASSERT_EQUAL_INT(LDA, str_to_mnemonic("LDA"));
	TEST_ASSERT_EQUAL_INT(LDX, str_to_mnemonic("LDX"));
	TEST_ASSERT_EQUAL_INT(LDY, str_to_mnemonic("LDY"));
	TEST_ASSERT_EQUAL_INT(LSR, str_to_mnemonic("LSR"));
	TEST_ASSERT_EQUAL_INT(NOP, str_to_mnemonic("NOP"));
	TEST_ASSERT_EQUAL_INT(ORA, str_to_mnemonic("ORA"));
	TEST_ASSERT_EQUAL_INT(PHA, str_to_mnemonic("PHA"));
	TEST_ASSERT_EQUAL_INT(PHP, str_to_mnemonic("PHP"));
	TEST_ASSERT_EQUAL_INT(PLA, str_to_mnemonic("PLA"));
	TEST_ASSERT_EQUAL_INT(PLP, str_to_mnemonic("PLP"));
	TEST_ASSERT_EQUAL_INT(ROL, str_to_mnemonic("ROL"));
	TEST_ASSERT_EQUAL_INT(ROR, str_to_mnemonic("ROR"));
	TEST_ASSERT_EQUAL_INT(RTI, str_to_mnemonic("RTI"));
	TEST_ASSERT_EQUAL_INT(RTS, str_to_mnemonic("RTS"));
	TEST_ASSERT_EQUAL_INT(SBC, str_to_mnemonic("SBC"));
	TEST_ASSERT_EQUAL_INT(SEC, str_to_mnemonic("SEC"));
	TEST_ASSERT_EQUAL_INT(SED, str_to_mnemonic("SED"));
	TEST_ASSERT_EQUAL_INT(SEI, str_to_mnemonic("SEI"));
	TEST_ASSERT_EQUAL_INT(STA, str_to_mnemonic("STA"));
	TEST_ASSERT_EQUAL_INT(STX, str_to_mnemonic("STX"));
	TEST_ASSERT_EQUAL_INT(STY, str_to_mnemonic("STY"));
	TEST_ASSERT_EQUAL_INT(TAX, str_to_mnemonic("TAX"));
	TEST_ASSERT_EQUAL_INT(TAY, str_to_mnemonic("TAY"));
	TEST_ASSERT_EQUAL_INT(TSX, str_to_mnemonic("TSX"));
	TEST_ASSERT_EQUAL_INT(TXA, str_to_mnemonic("TXA"));
	TEST_ASSERT_EQUAL_INT(TXS, str_to_mnemonic("TXS"));
	TEST_ASSERT_EQUAL_INT(TYA, str_to_mnemonic("TYA"));

	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("TXSABCDEFG"));
	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("ABC"));
	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("13245"));
	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("KDSJFLSFJSKFJKSLJ"));
	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("JANUS"));
	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, str_to_mnemonic("STLOOP"));
}

void test_get_addr_bitfield(void)
{
	TEST_ASSERT_EQUAL_INT(ADC_BITFIELD, get_addr_bitfield(ADC));
	TEST_ASSERT_EQUAL_INT(AND_BITFIELD, get_addr_bitfield(AND));
	TEST_ASSERT_EQUAL_INT(ASL_BITFIELD, get_addr_bitfield(ASL));
	TEST_ASSERT_EQUAL_INT(BCC_BITFIELD, get_addr_bitfield(BCC));
	TEST_ASSERT_EQUAL_INT(BCS_BITFIELD, get_addr_bitfield(BCS));
	TEST_ASSERT_EQUAL_INT(BEQ_BITFIELD, get_addr_bitfield(BEQ));
	TEST_ASSERT_EQUAL_INT(BIT_BITFIELD, get_addr_bitfield(BIT));
	TEST_ASSERT_EQUAL_INT(BMI_BITFIELD, get_addr_bitfield(BMI));
	TEST_ASSERT_EQUAL_INT(BNE_BITFIELD, get_addr_bitfield(BNE));
	TEST_ASSERT_EQUAL_INT(BPL_BITFIELD, get_addr_bitfield(BPL));
	TEST_ASSERT_EQUAL_INT(BRK_BITFIELD, get_addr_bitfield(BRK));
	TEST_ASSERT_EQUAL_INT(BVC_BITFIELD, get_addr_bitfield(BVC));
	TEST_ASSERT_EQUAL_INT(BVS_BITFIELD, get_addr_bitfield(BVS));
	TEST_ASSERT_EQUAL_INT(CLC_BITFIELD, get_addr_bitfield(CLC));
	TEST_ASSERT_EQUAL_INT(CLD_BITFIELD, get_addr_bitfield(CLD));
	TEST_ASSERT_EQUAL_INT(CLI_BITFIELD, get_addr_bitfield(CLI));
	TEST_ASSERT_EQUAL_INT(CLV_BITFIELD, get_addr_bitfield(CLV));
	TEST_ASSERT_EQUAL_INT(CMP_BITFIELD, get_addr_bitfield(CMP));
	TEST_ASSERT_EQUAL_INT(CPX_BITFIELD, get_addr_bitfield(CPX));
	TEST_ASSERT_EQUAL_INT(CPY_BITFIELD, get_addr_bitfield(CPY));
	TEST_ASSERT_EQUAL_INT(DEC_BITFIELD, get_addr_bitfield(DEC));
	TEST_ASSERT_EQUAL_INT(DEX_BITFIELD, get_addr_bitfield(DEX));
	TEST_ASSERT_EQUAL_INT(DEY_BITFIELD, get_addr_bitfield(DEY));
	TEST_ASSERT_EQUAL_INT(EOR_BITFIELD, get_addr_bitfield(EOR));
	TEST_ASSERT_EQUAL_INT(INC_BITFIELD, get_addr_bitfield(INC));
	TEST_ASSERT_EQUAL_INT(INX_BITFIELD, get_addr_bitfield(INX));
	TEST_ASSERT_EQUAL_INT(INY_BITFIELD, get_addr_bitfield(INY));
	TEST_ASSERT_EQUAL_INT(JMP_BITFIELD, get_addr_bitfield(JMP));
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, get_addr_bitfield(JSR));
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, get_addr_bitfield(LDA));
	TEST_ASSERT_EQUAL_INT(LDX_BITFIELD, get_addr_bitfield(LDX));
	TEST_ASSERT_EQUAL_INT(LDY_BITFIELD, get_addr_bitfield(LDY));
	TEST_ASSERT_EQUAL_INT(LSR_BITFIELD, get_addr_bitfield(LSR));
	TEST_ASSERT_EQUAL_INT(NOP_BITFIELD, get_addr_bitfield(NOP));
	TEST_ASSERT_EQUAL_INT(ORA_BITFIELD, get_addr_bitfield(ORA));
	TEST_ASSERT_EQUAL_INT(PHA_BITFIELD, get_addr_bitfield(PHA));
	TEST_ASSERT_EQUAL_INT(PHP_BITFIELD, get_addr_bitfield(PHP));
	TEST_ASSERT_EQUAL_INT(PLA_BITFIELD, get_addr_bitfield(PLA));
	TEST_ASSERT_EQUAL_INT(PLP_BITFIELD, get_addr_bitfield(PLP));
	TEST_ASSERT_EQUAL_INT(ROL_BITFIELD, get_addr_bitfield(ROL));
	TEST_ASSERT_EQUAL_INT(ROR_BITFIELD, get_addr_bitfield(ROR));
	TEST_ASSERT_EQUAL_INT(RTI_BITFIELD, get_addr_bitfield(RTI));
	TEST_ASSERT_EQUAL_INT(RTS_BITFIELD, get_addr_bitfield(RTS));
	TEST_ASSERT_EQUAL_INT(SBC_BITFIELD, get_addr_bitfield(SBC));
	TEST_ASSERT_EQUAL_INT(SEC_BITFIELD, get_addr_bitfield(SEC));
	TEST_ASSERT_EQUAL_INT(SED_BITFIELD, get_addr_bitfield(SED));
	TEST_ASSERT_EQUAL_INT(SEI_BITFIELD, get_addr_bitfield(SEI));
	TEST_ASSERT_EQUAL_INT(STA_BITFIELD, get_addr_bitfield(STA));
	TEST_ASSERT_EQUAL_INT(STX_BITFIELD, get_addr_bitfield(STX));
	TEST_ASSERT_EQUAL_INT(STY_BITFIELD, get_addr_bitfield(STY));
	TEST_ASSERT_EQUAL_INT(TAX_BITFIELD, get_addr_bitfield(TAX));
	TEST_ASSERT_EQUAL_INT(TAY_BITFIELD, get_addr_bitfield(TAY));
	TEST_ASSERT_EQUAL_INT(TSX_BITFIELD, get_addr_bitfield(TSX));
	TEST_ASSERT_EQUAL_INT(TXA_BITFIELD, get_addr_bitfield(TXA));
	TEST_ASSERT_EQUAL_INT(TXS_BITFIELD, get_addr_bitfield(TXS));
	TEST_ASSERT_EQUAL_INT(TYA_BITFIELD, get_addr_bitfield(TYA));

	TEST_ASSERT_EQUAL_INT(ERROR_BITFIELD_NOT_FOUND, get_addr_bitfield(123));
	TEST_ASSERT_EQUAL_INT(ERROR_BITFIELD_NOT_FOUND, get_addr_bitfield(-1));
	TEST_ASSERT_EQUAL_INT(ERROR_BITFIELD_NOT_FOUND, get_addr_bitfield(0xFFFF));
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_instr);
	RUN_TEST(test_str_to_mnemonic);
	RUN_TEST(test_get_addr_bitfield);

	return UNITY_END();
}