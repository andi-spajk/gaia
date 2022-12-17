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
	const char *buffer;

	const char *indirect = "ADC ($FF)\n";
	buffer = indirect;
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

	const char *indirect_y = "\t\tLDX\t($B0),Y\n";
	buffer = indirect_y;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label = "\t\tBCC\tLABEL\n";
	buffer = label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *immediate = "ADC #255\n";
	buffer = immediate;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *bad = "LDA LDX LDY LOLOL\n";
	buffer = bad;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *really_bad = "LDX (),\tX LABEL\n";
	buffer = really_bad;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_label_tree(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token **seq = lexer->sequence;
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

	const char *constant = "PATTERN = $2000\n";
	buffer = constant;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	TEST_ASSERT_EQUAL_INT(0x2000, seq[2]->value);
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label_operand = "REG CMP (STR),Y\n";
	buffer = label_operand;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *just_label = "PATTERNMATCH\n";
	buffer = just_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *too_many = "LABEL1 LABEL2 STLOOP JANUS YOYOYOEOYEOYOEYO\n";
	buffer = too_many;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *triple = "sexy === $ffff\n";
	buffer = triple;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_parse_instr_tree);
	RUN_TEST(test_parse_label_tree);

	return UNITY_END();
}