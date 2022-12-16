#include <string.h>

#include "../../unity/unity.h"
#include "bitfields.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"

// lexer.c has the same define, but it's not in lexer.h because the other
// modules don't need it
// therefore I must redefine MAX_TOKENS here just for this test file
#define MAX_TOKENS 8

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_lexer(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	for (int i = 0; i < MAX_TOKENS; i++) {
		TEST_ASSERT_NOT_NULL(lexer->sequence[i]);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[i]->type);
		TEST_ASSERT_NULL(lexer->sequence[i]->str);
		TEST_ASSERT_EQUAL_INT(0U, lexer->sequence[i]->value);
	}
	destroy_lexer(lexer);
}

void test_init_token_str(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	TEST_ASSERT_NULL(tk->str);

	init_token_str(tk, "label");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(5, strlen(tk->str));

	init_token_str(tk, "SEARCH");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(6, strlen(tk->str));

	init_token_str(tk, "CALC");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(4, strlen(tk->str));

	init_token_str(tk, "test_init_token_str");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(19, strlen(tk->str));

	destroy_token(tk);
}

void test_add_token(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	TEST_ASSERT_EQUAL_INT(0, lexer->curr);

	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	tk->type = TOKEN_LABEL;
	TEST_ASSERT_NOT_NULL(init_token_str(tk, "CHKEND"));
	tk->value = 0x1000U;

	// 1st insertion
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	struct Token *prev = lexer->sequence[lexer->curr-1];

	// check if inserted properly
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, prev->type);
	TEST_ASSERT_EQUAL_STRING("CHKEND", prev->str);
	TEST_ASSERT_EQUAL_INT(0x1000U, prev->value);
	// lexer index updates from 0 to 1 after insertion
	TEST_ASSERT_EQUAL_INT(1, lexer->curr);

	// now max out the sequence array
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(2, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(3, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(4, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(5, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(6, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(7, lexer->curr);
	TEST_ASSERT_NOT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(8, lexer->curr);

	// all tokens in the sequence should be identical
	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, curr->type);
		TEST_ASSERT_EQUAL_STRING("CHKEND", curr->str);
		TEST_ASSERT_EQUAL_INT(0x1000U, curr->value);
	}

	// this insertion should fail and return NULL
	TEST_ASSERT_NULL(add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(8, lexer->curr);

	destroy_token(tk);
	destroy_lexer(lexer);
}

void test_lex_literal(void)
{
	struct Token *tk = init_token();

	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "abc"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "@!#$"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, ";"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, ""));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "\t\n"));

	/*
	HEXADECIMAL LEXING
	*/
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$1234"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1234, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$FACE\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFACE, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$DEAD\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xDEAD, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$A00B\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xA00B, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$0BEE"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xBEE, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$005A"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x5A, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$0001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$0A0 ; lol comment"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0A0, tk->value);
	// lowercase
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$face\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFACE, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "$0000dead\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xDEAD, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$bee"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xBEE, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "$5a"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x5A, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$ace\t\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xACE, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "$ff"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF, tk->value);
	// more
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "$ab\t;AAAAAAAAAA\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xab, tk->value);
	TEST_ASSERT_EQUAL_INT(1, lex_literal(tk, "$1"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "$0c"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xC, tk->value);
	TEST_ASSERT_EQUAL_INT(10, lex_literal(tk, "$000000000a"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xA, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$0000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "$00000000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);

	// errors
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$FFFFFFF"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$1234567890ABCDEF"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$10000"));

	/*
	BINARY LEXING
	*/
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "%11111111"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "%01010001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x51, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "%00000000\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "%10000001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x81, tk->value);
	TEST_ASSERT_EQUAL_INT(16, lex_literal(tk, "%1111111100001110"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF0E, tk->value);
	TEST_ASSERT_EQUAL_INT(11, lex_literal(tk, "%10101010101\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x555, tk->value);
	TEST_ASSERT_EQUAL_INT(1, lex_literal(tk, "%0"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "%00000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(8, lex_literal(tk, "%00001001\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x9, tk->value);
	TEST_ASSERT_EQUAL_INT(20, lex_literal(tk, "%00001111111111111111"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFFFF, tk->value);

	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "%10001111111111111111"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "%111111111111111111111111111111"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk,
	                      "%1010101010101010101010101010101010101010101010101010110101010101"));

	/*
	DECIMAL LEXING
	*/
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "1234"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(1234, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "65535"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(65535, tk->value);
	TEST_ASSERT_EQUAL_INT(1, lex_literal(tk, "0"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "25                      "));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(25, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "800!!!!!!SDJGHJSDHFSDKVNSFULM"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(800, tk->value);
	TEST_ASSERT_EQUAL_INT(12, lex_literal(tk, "000000000100"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(100, tk->value);

	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "123456789"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "65536"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "A"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "ffff"));

	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "$"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "%"));

	destroy_token(tk);
}

void test_lex_instruction(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);

	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	init_token_str(tk, "ADC");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(ADC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(ADC_BITFIELD, instr->addr_bitfield);

	init_token_str(tk, "JSR");
	TEST_ASSERT_NOT_NULL(tk->str);

	// proceed with tests as expected
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(JSR, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, instr->addr_bitfield);

	init_token_str(tk, "BEQ");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(BEQ, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BEQ_BITFIELD, instr->addr_bitfield);

	init_token_str(tk, "badinstruction");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(ERROR_INSTRUCTION_NOT_FOUND, lex_instruction(tk, instr));
	// instr should NOT be modified if no mnemonic was found
	TEST_ASSERT_EQUAL_INT(BEQ, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BEQ_BITFIELD, instr->addr_bitfield);

	init_token_str(tk, "LDA");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);

	destroy_token(tk);
	destroy_instruction(instr);
}

void test_lex_text(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);

	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	char *source_line = "LABEL\tJSR MORE_LabeL\n";

	char *buffer = source_line;
	TEST_ASSERT_EQUAL_INT(5, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("LABEL", tk->str);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitfield);
	buffer += 5;
	buffer++;  // lex_text() can't skip leading whitespace
	           // but lex_line() will be able to

	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_STRING("JSR", tk->str);
	TEST_ASSERT_EQUAL_INT(JSR, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, instr->addr_bitfield);

	buffer += 3;
	buffer++;
	TEST_ASSERT_EQUAL_INT(10, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	// this assembler is case-insensitive!
	TEST_ASSERT_EQUAL_STRING("MORE_LABEL", tk->str);
	// instr should NOT have been modified if no mnemonic was found
	TEST_ASSERT_EQUAL_INT(JSR, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, instr->addr_bitfield);

	reset_instruction(instr);
	char *bad_source_line = "bad&&#label      LDA #$01";
//                               012345678901234567890123
	buffer = bad_source_line;
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_text(buffer, tk, instr));
	buffer += 17;
	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_STRING("LDA", tk->str);
	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);

	// max label length is 63 chars
	char *too_long_label = "abcdefgh2bcdefgh3bcdefgh4bcdefgh5bcdefgh6bcdefgh7bcdefgh8bcdefgh BEQ wtf";
	buffer = too_long_label;
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_LONG_LABEL, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("LDA", tk->str);

	char *just_enough =    "abcdefgh2bcdefgh3bcdefgh4bcdefgh5bcdefgh6bcdefgh7bcdefgh8bcdefg BEQ wtf";
	buffer = just_enough;
	TEST_ASSERT_EQUAL_INT(63, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("ABCDEFGH2BCDEFGH3BCDEFGH4BCDEFGH5BCDEFGH6BCDEFGH7BCDEFGH8BCDEFG", tk->str);

//                          01234567890 1
	char *x_register = "LDA ($00,X)\n";
	buffer = x_register + 9;
	TEST_ASSERT_EQUAL_INT(1, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_X_REGISTER, tk->type);

//                           012345678901 2
	char *y_register = "\tSTA $0200,Y\n";
	buffer = y_register + 11;
	TEST_ASSERT_EQUAL_INT(1, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, tk->type);

	destroy_token(tk);
	destroy_instruction(instr);
}

void test_lex(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);

	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

//                              01234 5 67 8901 2
	char *constant_label = "array\t\t=\t$30\n";
	char *buffer = constant_label;
	TEST_ASSERT_EQUAL_INT(5, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);
	buffer += 5;

	buffer += 2;
	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_SIGN, tk->type);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);
	buffer += 1;

	buffer += 1;
	TEST_ASSERT_EQUAL_INT(2, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x30, tk->value);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);

//                           01234 5 678901234567 8 9012345678901234567890 1
	char *source_line = "SORT8\t\tlda (array),Y\t\t; FETCH ELEMENT COUNT\n";
	buffer = source_line;

	TEST_ASSERT_EQUAL_INT(5, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);
	buffer += 5;

	buffer += 2;
	TEST_ASSERT_EQUAL_INT(3, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);
	buffer += 3;

	buffer += 1;
	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_OPEN_PARENTHESIS, tk->type);
	// instr should be unchanged
	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(5, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("ARRAY", tk->str);
	buffer += 5;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_CLOSE_PARENTHESIS, tk->type);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, tk->type);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, tk->type);

//                              0123456789
	char *another_source = "LDA #$0001\n";
	buffer = another_source + 4;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_IMMEDIATE, tk->type);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(4, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(1, tk->value);

	destroy_token(tk);
	destroy_instruction(instr);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_lexer);
	RUN_TEST(test_init_token_str);
	RUN_TEST(test_add_token);
	RUN_TEST(test_lex_literal);
	RUN_TEST(test_lex_instruction);
	RUN_TEST(test_lex_text);
	RUN_TEST(test_lex);

	return UNITY_END();
}