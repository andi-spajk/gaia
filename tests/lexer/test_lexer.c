#include <string.h>

#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"

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

void test_add_token(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	TEST_ASSERT_EQUAL_INT(0, lexer->curr);

	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	tk->type = TOKEN_LABEL;
	TEST_ASSERT_NOT_NULL(init_token_str(tk, "CHKEND"));
	// strncpy(tk->str, "CHKEND", 7);
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
	HEXADECIMAL TOKENIZATION
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
	BINARY TOKENIZATION
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
	DECIMAL TOKENIZATION
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

	destroy_token(tk);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_lexer);
	RUN_TEST(test_add_token);
	RUN_TEST(test_lex_literal);

	return UNITY_END();
}