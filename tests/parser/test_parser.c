#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

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

	const char *label_operand = "REG\t\tCMP\t(STR),Y\t;stuff\n";
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

void test_parse_token_sequence(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

	const char *source_line = "DISP\t\tLDX\t#$1F\n";
	buffer = source_line;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *source_line2 = "AND #$7F\n";
	buffer = source_line2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *source_line3 = "INCHEK\t=\t$B4\n";
	buffer = source_line3;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *address_label = "NEXT\t\tLDA PATTERN,X\n";
	buffer = address_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label_operand = "REG\t\tCMP\t(STR),Y\t;stuff\n";
	buffer = label_operand;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *indirect_x = "LDA ($00,X)\n";
	buffer = indirect_x;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *address_label2 = "STAR\t\tINX\n";
	buffer = address_label2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_label_declaration(void)
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

	const char *address_label = "NEXT\t\tLDA PATTERN,X\n";
	buffer = address_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x4));
	TEST_ASSERT_EQUAL_INT(0x4, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x4, lexer->sequence[0]->value);
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *constant_label = "flag\t=\t$32\n";
	buffer = constant_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(0, lexer->sequence[0]->value);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x32, lexer->sequence[0]->value);
	TEST_ASSERT_EQUAL_INT(0x32, search_symbol(symtab, lexer->sequence[0]->str));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *lone_label = "PATTERNMATCH\n";
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x0, lexer->sequence[0]->value);
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label_redefinition = "flag\t\t=\t$1234\n";
	buffer = label_redefinition;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x0));
	// old value should still be saved in symtab
	TEST_ASSERT_EQUAL_INT(0x32, search_symbol(symtab, lexer->sequence[0]->str));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *label_redefinition2 = "NEXT TXA\n";
	buffer = label_redefinition2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x7));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_get_operand(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

//                                   0      1    23
	const char *label_operand = "REG\t\tCMP\t(STR),Y\t;stuff\n";
	buffer = label_operand;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], get_operand(lexer));
	reset_lexer(lexer);

//                                 0       1    23
	const char *source_line = "DISP\t\tLDX\t#$1F\n";
	buffer = source_line;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], get_operand(lexer));
	reset_lexer(lexer);

//                                   0       1   2
	const char *address_label = "NEXT\t\tLDA PATTERN,X\n";
	buffer = address_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], get_operand(lexer));
	reset_lexer(lexer);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_label_operand(void)
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
	int pc = 0x0;

/*
example program for testing
byte count on the far left side
                LDX     PATTERN

        PATTERN =       $2000

3       NEXT    LDA     PATTERN,X
2               BEQ     NEXT
3               JSR     NEXT
2               BCC     STAR
3               JMP     STAR
1       STAR    INX
*/

	const char *illegal_for_ref = "\tLDX\tPATTERN\n";
	buffer = illegal_for_ref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "PATTERN"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE,
	                      parse_label_operand(lexer->sequence[1], instr, symtab));
	reset_lexer(lexer);
	reset_instruction(instr);

	const char *constant_label = "PATTERN = $2000\n";
	buffer = constant_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(0x2000, search_symbol(symtab, "PATTERN"));
	reset_lexer(lexer);
	reset_instruction(instr);

//                                   0       1   2
	const char *address_label = "NEXT\t\tLDA PATTERN,X\n";
	buffer = address_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer->sequence[2], instr, symtab));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, "NEXT"));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 3;

	const char *branch = "\t\tBEQ NEXT\n";
	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(lexer->sequence[1], instr, symtab));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, lexer->sequence[1]->str));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 2;

	const char *jump = "\t\tJSR NEXT\t; comment\n";
	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(lexer->sequence[1], instr, symtab));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, lexer->sequence[1]->str));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 3;

	const char *branch_for_ref = "BCC\tSTAR\n";
	buffer = branch_for_ref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(lexer->sequence[1], instr, symtab));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 2;

	const char *jump_for_ref = "JMP STAR\n";
	buffer = jump_for_ref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(lexer->sequence[1], instr, symtab));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 3;

	// this instr is at byte index 13 (ie 14th byte)
	const char *address_label2 = "STAR\t\tINX\n";
	buffer = address_label2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_token_sequence(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(0xD, search_symbol(symtab, lexer->sequence[0]->str));
	reset_lexer(lexer);
	reset_instruction(instr);
	pc += 1;

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_parse_instr_tree);
	RUN_TEST(test_parse_label_tree);
	RUN_TEST(test_parse_token_sequence);
	RUN_TEST(test_parse_label_declaration);
	RUN_TEST(test_get_operand);
	RUN_TEST(test_parse_label_operand);

	return UNITY_END();
}