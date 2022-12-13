#include "../../unity/unity.h"

#include "opcode.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_instr(void)
{
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	TEST_ASSERT_EQUAL_INT(ILLEGAL_MNEMONIC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_field);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_flag);
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

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_instr);
	RUN_TEST(test_str_to_mnemonic);

	return UNITY_END();
}