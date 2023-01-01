#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../unity/unity.h"
#include "bitfields.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_lexer(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	for (int i = 0; i < MAX_TOKENS; i++) {
		TEST_ASSERT_NOT_NULL(lexer->sequence[i]);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[i]->type);
		TEST_ASSERT_NOT_NULL(lexer->sequence[i]->str);
		TEST_ASSERT_EQUAL_INT(0U, lexer->sequence[i]->value);
	}
	destroy_lexer(lexer);
	destroy_lexer(NULL);
}

void test_token_strcpy(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	TEST_ASSERT_NOT_NULL(tk->str);

	// token_strcpy() doesnt convert case
	// however we only call token_strcpy() after we've validated
	// a token, which is when we add the token to the lexer
	token_strcpy(tk, "LABEL");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(5, strlen(tk->str));
	TEST_ASSERT_EQUAL_STRING("LABEL\0", tk->str);

	token_strcpy(tk, "SEARCH");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(6, strlen(tk->str));
	TEST_ASSERT_EQUAL_STRING("SEARCH\0", tk->str);

	token_strcpy(tk, "CALC");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(4, strlen(tk->str));
	TEST_ASSERT_EQUAL_STRING("CALC\0", tk->str);

	token_strcpy(tk, "TEST_TOKEN_STRCPY");
	TEST_ASSERT_NOT_NULL(tk->str);
	TEST_ASSERT_EQUAL_INT(17, strlen(tk->str));
	TEST_ASSERT_EQUAL_STRING("TEST_TOKEN_STRCPY\0", tk->str);

	TEST_ASSERT_EQUAL_INT(0, token_strcpy(tk, ""));
	// same string from previous call to token_strcpy should still exist
	TEST_ASSERT_EQUAL_STRING("TEST_TOKEN_STRCPY\0", tk->str);
	TEST_ASSERT_EQUAL_INT(17, strlen(tk->str));
	TEST_ASSERT_EQUAL_INT(0, token_strcpy(tk, NULL));
	TEST_ASSERT_EQUAL_STRING("TEST_TOKEN_STRCPY\0", tk->str);
	TEST_ASSERT_EQUAL_INT(17, strlen(tk->str));

	destroy_token(tk);
}

void test_token_strncpy(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	const char *buffer;

	buffer = "LABEL1 TXA\n";
	token_strncpy(tk, buffer, 6);
	TEST_ASSERT_EQUAL_STRING("LABEL1", tk->str);

	buffer = "Loop\tCMP\t$01\n";
	token_strncpy(tk, buffer, 4);
	TEST_ASSERT_EQUAL_STRING("LOOP", tk->str);

	buffer = "LoNeLaBeL\n";
	token_strncpy(tk, buffer, 9);
	TEST_ASSERT_EQUAL_STRING("LONELABEL", tk->str);

	buffer = "label2,X)\n";
	token_strncpy(tk, buffer, 6);
	TEST_ASSERT_EQUAL_STRING("LABEL2", tk->str);

	buffer = "ORA (SOMETHING),Y\n";
	token_strncpy(tk, buffer, 3);
	TEST_ASSERT_EQUAL_STRING("ORA", tk->str);

	buffer = "INDIRECT)\n";
	token_strncpy(tk, buffer, 8);
	TEST_ASSERT_EQUAL_STRING("INDIRECT", tk->str);

	buffer = "PLA ;hi";
	token_strncpy(tk, buffer, 3);
	TEST_ASSERT_EQUAL_STRING("PLA", tk->str);

	buffer = "LDX\tNUM\n";
	token_strncpy(tk, buffer, 3);
	TEST_ASSERT_EQUAL_STRING("LDX", tk->str);

	buffer = "X\n";
	token_strncpy(tk, buffer, 1);
	TEST_ASSERT_EQUAL_STRING("X", tk->str);

	buffer = "Y\n";
	token_strncpy(tk, buffer, 1);
	TEST_ASSERT_EQUAL_STRING("Y", tk->str);

	buffer = "XNOTREGISTER\t;comment\n";
	token_strncpy(tk, buffer, 12);
	TEST_ASSERT_EQUAL_STRING("XNOTREGISTER", tk->str);

	buffer = "YNOTREGISTER    ; comment again\n";
	token_strncpy(tk, buffer, 12);
	TEST_ASSERT_EQUAL_STRING("YNOTREGISTER", tk->str);

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
	TEST_ASSERT_EQUAL_INT(6, token_strcpy(tk, "CHKEND"));
	tk->value = 0x1000U;

	// 1st insertion
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	struct Token *prev = lexer->sequence[lexer->curr-1];

	// check if inserted properly
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, prev->type);
	TEST_ASSERT_EQUAL_STRING("CHKEND", prev->str);
	TEST_ASSERT_EQUAL_INT(0x1000U, prev->value);
	// lexer index updates from 0 to 1 after insertion
	TEST_ASSERT_EQUAL_INT(1, lexer->curr);

	// now max out the sequence array
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(2, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(3, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(4, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(5, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(6, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(7, lexer->curr);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSERTION_SUCCESS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(8, lexer->curr);

	// all tokens in the sequence should be identical
	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, curr->type);
		TEST_ASSERT_EQUAL_STRING("CHKEND", curr->str);
		TEST_ASSERT_EQUAL_INT(0x1000U, curr->value);
	}

	// all tokens are filled so this insertion should fail
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_MANY_TOKENS, add_token(lexer, tk));
	TEST_ASSERT_EQUAL_INT(8, lexer->curr);

	destroy_token(tk);
	destroy_lexer(lexer);
}

void test_lex_literal(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);

	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "abc"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "@!#$"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, ";"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, ""));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "\t\n"));

	//
	// HEXADECIMAL LEXING
	//
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$1234"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1234, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$FACE\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFACE, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$DEAD\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xDEAD, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$A00B\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xA00B, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$0BEE"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xBEE, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$005A"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x5A, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$0001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$0A0 ; lol comment"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0A0, tk->value);
	// lowercase
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$face\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFACE, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "$0000dead\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xDEAD, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$bee"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xBEE, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$5a"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x5A, tk->value);
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "$ace\t\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xACE, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$ff"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF, tk->value);
	// more
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$ab\t;AAAAAAAAAA\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xab, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "$1"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x1, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "$0c"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xC, tk->value);
	TEST_ASSERT_EQUAL_INT(11, lex_literal(tk, "$000000000a"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xA, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$0000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "$00000000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(17, lex_literal(tk, "$0000000000000000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);

	// errors
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$FFFFFFF"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$1234567890ABCDEF"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "$10000"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "$\n"));

	//
	// BINARY LEXING
	//
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%11111111"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%01010001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x51, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%00000000\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%10000001"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x81, tk->value);
	TEST_ASSERT_EQUAL_INT(17, lex_literal(tk, "%1111111100001110"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFF0E, tk->value);
	TEST_ASSERT_EQUAL_INT(12, lex_literal(tk, "%10101010101\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x555, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "%0"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(6, lex_literal(tk, "%00000"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x0, tk->value);
	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%00001001\t\t"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x9, tk->value);
	TEST_ASSERT_EQUAL_INT(21, lex_literal(tk, "%00001111111111111111"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xFFFF, tk->value);

	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "%10001111111111111111"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "%111111111111111111111111111111"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk,
	                      "%1010101010101010101010101010101010101010101010101010110101010101"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "%\n"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "%101002\n"));

	//
	// DECIMAL LEXING
	//
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "1234"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(1234, tk->value);
	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "65535"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(65535, tk->value);
	TEST_ASSERT_EQUAL_INT(1, lex_literal(tk, "0"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0, tk->value);
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "25"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(25, tk->value);
	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "800"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(800, tk->value);
	TEST_ASSERT_EQUAL_INT(12, lex_literal(tk, "000000000100"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(100, tk->value);

	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "123456789"));
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_literal(tk, "65536"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "A"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "ffff"));

	//
	// detect VALID end of token
	//
	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "25             "));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(25, tk->value);

	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "128\t;hi\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(128, tk->value);

	TEST_ASSERT_EQUAL_INT(3, lex_literal(tk, "800\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(800, tk->value);

	TEST_ASSERT_EQUAL_INT(5, lex_literal(tk, "$ffff,X\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0xffff, tk->value);

	TEST_ASSERT_EQUAL_INT(9, lex_literal(tk, "%10101111),Y\t;comment\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0b10101111, tk->value);

	TEST_ASSERT_EQUAL_INT(2, lex_literal(tk, "64;comment\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(64, tk->value);

	// syntactically invalid, but lexically valid
	TEST_ASSERT_EQUAL_INT(4, lex_literal(tk, "5555(wtf),Y\n"));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(5555, tk->value);

	//
	// INVALID token chars
	//
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "800!!!!!!SDJGHJSDHFSDKVNSFULM"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "$facq"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "%10102"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_literal(tk, "%ABCD"));

	destroy_token(tk);
}

void test_lex_instruction(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);

	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

	token_strncpy(tk, "ADC", 3);
	TEST_ASSERT_EQUAL_STRING("ADC", tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(ADC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(ADC_BITFIELD, instr->addr_bitfield);

	// remember, token_strncpy() will convert to uppercase
	token_strncpy(tk, "jsr", 3);
	TEST_ASSERT_EQUAL_STRING("JSR", tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(JSR, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, instr->addr_bitfield);

	token_strncpy(tk, "bEq", 3);
	TEST_ASSERT_EQUAL_STRING("BEQ", tk->str);
	TEST_ASSERT_EQUAL_INT(3, lex_instruction(tk, instr));
	TEST_ASSERT_EQUAL_INT(BEQ, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BEQ_BITFIELD, instr->addr_bitfield);

	token_strncpy(tk, "badinstruction", 14);
	TEST_ASSERT_EQUAL_STRING("BADINSTRUCTION", tk->str);
	TEST_ASSERT_EQUAL_INT(ERROR_INSTRUCTION_NOT_FOUND, lex_instruction(tk, instr));
	// instr should NOT be modified if no mnemonic was found
	TEST_ASSERT_EQUAL_INT(BEQ, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BEQ_BITFIELD, instr->addr_bitfield);

	token_strncpy(tk, "LdA", 3);
	TEST_ASSERT_EQUAL_STRING("LDA", tk->str);
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
	const char *buffer;

	buffer = "LABEL1 TXA\n";
	TEST_ASSERT_EQUAL_INT(6, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("LABEL1", tk->str);

	buffer = "Loop\tCMP\t$01\n";
	TEST_ASSERT_EQUAL_INT(4, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	// lex_text() converts case
	TEST_ASSERT_EQUAL_STRING("LOOP", tk->str);

	buffer = "LoNeLaBeL\n";
	TEST_ASSERT_EQUAL_INT(9, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("LONELABEL", tk->str);

	buffer = "label2,X)\n";
	TEST_ASSERT_EQUAL_INT(6, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("LABEL2", tk->str);

	buffer = "INDIRECT)\n";
	TEST_ASSERT_EQUAL_INT(8, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("INDIRECT", tk->str);

	buffer = "SORT;comment\n";
	TEST_ASSERT_EQUAL_INT(4, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("SORT", tk->str);

	buffer = "ORA (SOMETHING),Y\n";
	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("ORA", tk->str);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_INT(ORA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(ORA_BITFIELD, instr->addr_bitfield);

	buffer = "PLA ;hi";
	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("PLA", tk->str);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_INT(PLA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(PLA_BITFIELD, instr->addr_bitfield);

	buffer = "LDX\tNUM\n";
	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("LDX", tk->str);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_INT(LDX, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDX_BITFIELD, instr->addr_bitfield);

	buffer = "X\n";
	TEST_ASSERT_EQUAL_INT(1, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("X", tk->str);
	TEST_ASSERT_EQUAL_INT(TOKEN_X_REGISTER, tk->type);

	buffer = "Y\n";
	TEST_ASSERT_EQUAL_INT(1, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("Y", tk->str);
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, tk->type);

	buffer = "XNOTREGISTER\t;comment\n";
	TEST_ASSERT_EQUAL_INT(12, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("XNOTREGISTER", tk->str);

	buffer = "YNOTREGISTER    ; comment again\n";
	TEST_ASSERT_EQUAL_INT(12, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("YNOTREGISTER", tk->str);

	char *eof_test = malloc(5 * sizeof(char));
	if (eof_test) {
		strncpy(eof_test, "EOF\n", 5);
		eof_test[3] = EOF;
		buffer = eof_test;
		TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
		TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
		TEST_ASSERT_EQUAL_STRING("EOF", tk->str);
		TEST_ASSERT_EQUAL_INT(EOF, buffer[3]);
		free(eof_test);
	}

	buffer = "LDA#$01\n";
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_text(buffer, tk, instr));

	reset_instruction(instr);

//                                 01234 567890123456789 0
	const char *source_line = "LABEL\tJSR MORE_LabeL\n";
	buffer = source_line;
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
	TEST_ASSERT_EQUAL_STRING("MORE_LABEL", tk->str);
	// instr should NOT have been modified if no mnemonic was found
	TEST_ASSERT_EQUAL_INT(JSR, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JSR_BITFIELD, instr->addr_bitfield);

	reset_instruction(instr);

//                                     012345678901234567890123
	const char *bad_source_line = "bad&&#label      LDA #$01";
	buffer = bad_source_line;
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_text(buffer, tk, instr));
	buffer += 17;
	TEST_ASSERT_EQUAL_INT(3, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, tk->type);
	TEST_ASSERT_EQUAL_STRING("LDA", tk->str);
	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);

	// max label length is 63 chars
	const char *too_long_label = "abcdefgh2bcdefgh3bcdefgh4bcdefgh5bcdefgh6bcdefgh7bcdefgh8bcdefgh BEQ wtf";
	buffer = too_long_label;
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_LONG_LABEL, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_STRING("LDA", tk->str);

	const char *just_enough =    "abcdefgh2bcdefgh3bcdefgh4bcdefgh5bcdefgh6bcdefgh7bcdefgh8bcdefg BEQ wtf";
	buffer = just_enough;
	TEST_ASSERT_EQUAL_INT(63, lex_text(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	TEST_ASSERT_EQUAL_STRING("ABCDEFGH2BCDEFGH3BCDEFGH4BCDEFGH5BCDEFGH6BCDEFGH7BCDEFGH8BCDEFG", tk->str);

	destroy_token(tk);
	destroy_instruction(instr);
}

void test_lex(void)
{
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

//                               0123456789
	const char *immediate = "#%10011001\n";
	buffer = immediate;
	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_IMMEDIATE, tk->type);
	TEST_ASSERT_EQUAL_STRING("#", tk->str);
	buffer++;

	TEST_ASSERT_EQUAL_INT(9, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0b10011001, tk->value);

//                              01234 5 67 8901 2
	const char *constant = "array\t\t=\t$30\n";
	buffer = constant;
	TEST_ASSERT_EQUAL_INT(5, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, tk->type);
	buffer += 5;

	buffer += 2;
	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_SIGN, tk->type);
	TEST_ASSERT_EQUAL_STRING("=", tk->str);
	buffer += 1;

	buffer += 1;
	TEST_ASSERT_EQUAL_INT(3, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, tk->type);
	TEST_ASSERT_EQUAL_INT(0x30, tk->value);

//                          01234 5 678901234567 8
	const char *indy = "SORT8\t\tlda (array),Y\t\t; FETCH ELEMENT COUNT\n";
	buffer = indy;
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
	TEST_ASSERT_EQUAL_STRING("(", tk->str);
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
	TEST_ASSERT_EQUAL_STRING(")", tk->str);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, tk->type);
	TEST_ASSERT_EQUAL_STRING(",", tk->str);
	buffer += 1;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, tk->type);
	TEST_ASSERT_EQUAL_STRING("Y", tk->str);

//                                    0123456789
	const char *another_source = "LDA 256,X\n";
	buffer = another_source + 8;

	TEST_ASSERT_EQUAL_INT(1, lex(buffer, tk, instr));
	TEST_ASSERT_EQUAL_INT(TOKEN_X_REGISTER, tk->type);
	TEST_ASSERT_EQUAL_STRING("X", tk->str);

	destroy_token(tk);
	destroy_instruction(instr);
}

void test_lex_line(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

	// lex_line() resets the lexer and instr for us, so we will not need to
	// call the resets ourselves

	const char *source_line = "SEARCH\t\tLDA\tBOARD,X\n";
	buffer = source_line;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// 5 tokens from index [0,4], so lexer->curr should be index 5
	TEST_ASSERT_EQUAL_INT(5, lexer->curr);

	TEST_ASSERT_EQUAL_INT(LDA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(LDA_BITFIELD, instr->addr_bitfield);

	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[0]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[1]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[2]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, lexer->sequence[3]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_X_REGISTER, lexer->sequence[4]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[5]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[6]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

	TEST_ASSERT_EQUAL_STRING("SEARCH", lexer->sequence[0]->str);
	TEST_ASSERT_EQUAL_STRING("BOARD", lexer->sequence[2]->str);

	const char *bad_line = "\t\tADC\tBCC\t; lol\n";
	buffer = bad_line;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(2, lexer->curr);

	// only the last instr should be saved
	TEST_ASSERT_EQUAL_INT(BCC, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BCC_BITFIELD, instr->addr_bitfield);

	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[0]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[1]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[2]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[3]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[4]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[5]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[6]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

	const char *really_bad_line = "LABEL LABEL2 JMP ADC (LMAO,X)\n";
	buffer = really_bad_line;
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_MANY_TOKENS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(8, lexer->curr);

	TEST_ASSERT_EQUAL_INT(TOKEN_X_REGISTER, lexer->sequence[7]->type);

	const char *y_register_example = "ELOOP\t\tCMP\tBK,Y\n";
	buffer = y_register_example;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(5, lexer->curr);

	TEST_ASSERT_EQUAL_INT(CMP, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(CMP_BITFIELD, instr->addr_bitfield);

	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[0]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[1]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[2]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, lexer->sequence[3]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, lexer->sequence[4]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[5]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[6]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

	const char *constant = "address\t=\t$1234\n";
	buffer = constant;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(3, lexer->curr);

	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);

	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[0]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_SIGN, lexer->sequence[1]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, lexer->sequence[2]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[3]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[4]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[5]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[6]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

	TEST_ASSERT_EQUAL_STRING("ADDRESS", lexer->sequence[0]->str);
	TEST_ASSERT_EQUAL_INT(0x1234, lexer->sequence[2]->value);

	// null-terminated string instead of newline and then null-terminated
	const char *indy = "L20\t\tSTA\t($FF),Y";
	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(7, lexer->curr);

	TEST_ASSERT_EQUAL_INT(STA, instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(STA_BITFIELD, instr->addr_bitfield);

	TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[0]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[1]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_OPEN_PARENTHESIS, lexer->sequence[2]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, lexer->sequence[3]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_CLOSE_PARENTHESIS, lexer->sequence[4]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_COMMA, lexer->sequence[5]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_Y_REGISTER, lexer->sequence[6]->type);
	TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

	TEST_ASSERT_EQUAL_STRING("L20", lexer->sequence[0]->str);
	TEST_ASSERT_EQUAL_INT(0xFF, lexer->sequence[3]->value);

	// EOF and then null-terminated
	char *eof_test = malloc(17 * sizeof(char));
	if (eof_test) {
		strncpy(eof_test, "EOFTEST LSR $0A", 17);
		eof_test[15] = EOF;
		buffer = eof_test;
		TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
		TEST_ASSERT_EQUAL_INT(3, lexer->curr);

		TEST_ASSERT_EQUAL_INT(LSR, instr->mnemonic);
		TEST_ASSERT_EQUAL_INT(LSR_BITFIELD, instr->addr_bitfield);

		TEST_ASSERT_EQUAL_INT(TOKEN_LABEL, lexer->sequence[0]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_INSTRUCTION, lexer->sequence[1]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_LITERAL, lexer->sequence[2]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[3]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[4]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[5]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[6]->type);
		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[7]->type);

		TEST_ASSERT_EQUAL_STRING("EOFTEST", lexer->sequence[0]->str);
		TEST_ASSERT_EQUAL_INT(0xA, lexer->sequence[2]->value);
		free(eof_test);
	}

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_lexer);
	RUN_TEST(test_token_strcpy);
	RUN_TEST(test_token_strncpy);
	RUN_TEST(test_add_token);
	RUN_TEST(test_lex_literal);
	RUN_TEST(test_lex_instruction);
	RUN_TEST(test_lex_text);
	RUN_TEST(test_lex);
	RUN_TEST(test_lex_line);

	return UNITY_END();
}