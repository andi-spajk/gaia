#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"

void setUp(void) {}
void tearDown(void) {}

void test_parse_instr_tree(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token **seq = lexer->sequence;
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	const char *indirect = "ADC ($FF)\n";
	const char *buffer = indirect;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *indirect_x = "LDA ($00,X)\n";
	buffer = indirect_x;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *indirect_y = "LDX ($B0),Y\n";
	buffer = indirect_y;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label = "BCC LABEL\n";
	buffer = label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *immediate = "ADC #255\n";
	buffer = immediate;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_parse_instr_tree);

	return UNITY_END();
}