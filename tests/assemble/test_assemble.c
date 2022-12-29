#include "../../unity/unity.h"

#include "addressing_modes.h"
#include "assemble.h"
#include "bitfields.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

void setUp(void) {}
void tearDown(void) {}

void test_assembly(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	const char *buffer;
	struct Token *operand;

	const char *line = "\t\tSBC\t($AA),Y\n";
	buffer = line;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SBC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(SBC_BITFIELD, instr->addr_bitfield);

	operand = find_operand(lexer);
	int operand_status = parse_operand(operand, instr, symtab);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, operand_status);

	int addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT_Y_INDEXED, addr_mask);

	instr->addr_bitflag = addr_mask & instr->addr_bitfield;
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT_Y_INDEXED, instr->addr_bitflag);

	TEST_ASSERT_EQUAL_INT(0xF1, assemble_SBC(instr->addr_bitflag));



	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_ADDRESSING_MODE, assemble_JSR(ADDR_MODE_INDIRECT));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_ADDRESSING_MODE, assemble_TXA(ADDR_MODE_ZERO_PAGE_X));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_ADDRESSING_MODE, assemble_BEQ(ADDR_MODE_IMMEDIATE));
	TEST_ASSERT_EQUAL_INT(0x45, assemble_EOR(ADDR_MODE_ZERO_PAGE));
	TEST_ASSERT_EQUAL_INT(0xA4, assemble_LDY(ADDR_MODE_ZERO_PAGE));
	TEST_ASSERT_EQUAL_INT(0x9D, assemble_STA(ADDR_MODE_ABSOLUTE_X));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_assembly);

	return UNITY_END();
}