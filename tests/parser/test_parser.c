#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

#include "exhaustive_lines.h"

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

	buffer = lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));
	buffer = jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_instr_tree(seq, 0));

	const char *bad = "LDA LDX LDY LOLOL\n";
	buffer = bad;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));

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
	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_abs;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));
	buffer = label_indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_tree(seq, 0));

	const char *too_many = "LABEL1 LABEL2 STLOOP JANUS YOYOYOEOYEOYOEYO\n";
	buffer = too_many;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));

	const char *triple = "sexy === $ffff\n";
	buffer = triple;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_instr_tree(seq, 0));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_line(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_abs;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	buffer = label_indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));

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

	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0xABCD, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0xABCD, lexer->sequence[0]->value);
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x7F, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x7F, lexer->sequence[0]->value);
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x1111, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x1111, lexer->sequence[0]->value);
	// let's set pc = 2
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x2, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x2, lexer->sequence[0]->value);

	const char *label_redefinition = "CONSTANT16\t\t=\t$1234\n";
	buffer = label_redefinition;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x2));
	// old value should still be saved in symtab
	TEST_ASSERT_EQUAL_INT(0x1111, search_symbol(symtab, lexer->sequence[0]->str));

	const char *label_redefinition2 = "LABEL1          TXA\n";
	buffer = label_redefinition2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x2, search_symbol(symtab, lexer->sequence[0]->str));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_find_operand(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	const char *buffer;

	buffer = lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(NULL, find_operand(lexer));
	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], find_operand(lexer));
	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(NULL, find_operand(lexer));
	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_abs;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], find_operand(lexer));
	buffer = label_ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));
	buffer = label_indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[3], find_operand(lexer));

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

	// set up constant labels
	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));

	// label operands that are not branch nor jump
	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));

	// no indirect instruction because that is always a jump!

	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));

/*
;--------------------------------------
; example program for testing

pc      LABEL1                                  ; 0 bytes
0                       INX                     ; 1 byte, 1 total byte
                        CPY     ADDRESS         ; illegal forward reference
1       RANDOM_LABEL    DEY                     ; 1 byte, 2 total bytes
2       BMI             LABEL1                  ; 2 bytes, 4 total bytes
4       BVS             L1                      ; 2, 6
6       JMP             LABEL1                  ; 3, 9
9       JSR             L3                      ; 3, 12
12      LABEL2          BCC     LABEL1          ; 2, 14
14      FORREF1         BVS     L19             ; 2, 16
16      LABEL3          JSR     LABEL2          ; 3, 19
19      FORREF2         JMP     L21             ; 3, 22
                                                ; 22
*/
	// the earlier tests already defined ADDRESS
	// so we must reset the symbol table
	destroy_symbol_table(symtab);
	symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	int pc = 0x0;

	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, "LABEL1"));

	// INX
	buffer = lone_instr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc += 1;

	// CPY ADDRESS
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "ADDRESS"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE,
	                      parse_label_operand(find_operand(lexer), instr, symtab));

	// RANDOM_LABEL DEY
	buffer = "RANDOM_LABEL\t\tDEY\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	pc += 1;

	// BMI LABEL1
	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// BVS L1
	buffer = branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// JMP LABEL1
	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// JSR L3
	buffer = jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// LABEL2 BCC LABEL1
	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// FORREF1 BVS L19
	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// LABEL3 JSR LABEL2
	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// FORREF2 JMP L21
	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(22, search_symbol(symtab, "L2"));

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
	RUN_TEST(test_parse_line);
	RUN_TEST(test_parse_label_declaration);
	RUN_TEST(test_find_operand);
	RUN_TEST(test_parse_label_operand);

	return UNITY_END();
}