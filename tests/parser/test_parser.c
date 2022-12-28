#include "../../unity/unity.h"
#include "addressing_modes.h"
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

	buffer = imp;
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
	buffer = label_imp;
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
	buffer = imp;
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
	buffer = label_imp;
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

	const char *bad = "LDA LDX LDY LOLOL\n";
	buffer = bad;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	const char *really_bad = "LDX (),\tX LABEL\n";
	buffer = really_bad;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	const char *too_many = "LABEL1 LABEL2 STLOOP JANUS YOYOYOEOYEOYOEYO\n";
	buffer = too_many;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	const char *triple = "sexy === $ffff\n";
	buffer = triple;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	const char *bad_label = "LDA X\n";  // parser reads X as the register
	buffer = bad_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));

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

	buffer = imp;
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
	buffer = label_imp;
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

	// test case-insensitivity
	buffer = "ADC constant8\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));

	buffer = "STX constant16\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));

	buffer = "\t\tSTY\taddress\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(find_operand(lexer), instr, symtab));
	/*
	;--------------------------------------
	; example program for parsing forward refs and branch/jumps

	pc      LABEL1                                  ; 0 bytes
	0                       INX                     ; 1 byte, 1 total byte
	                        CPY     ADDRESS         ; illegal forward reference
	1       RANDOM_LABEL    DEY                     ; 1 byte, 2 total bytes
	2                       BMI     LABEL1          ; 2 bytes, 4 total bytes
	4                       BVS     L1              ; 2, 6
	6                       JMP     LABEL1          ; 3, 9
	9                       JSR     L3              ; 3, 12
	12      LABEL2          BCC     LABEL1          ; 2, 14
	14      FORREF1         BVC     L19             ; 2, 16
	16      LABEL3          JSR     LABEL2          ; 3, 19
	19      FORREF2         JMP     L21             ; 3, 22
	22      L2              STA     $AA             ; 2, 24
	24                      JMP     (WHERE)         ; 3, 27
	27      FORREF3         JMP     (WHERE)         ; 3, 30
	                                                ; 30
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
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, "LABEL1"));

	// INX
	buffer = imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc += 1;

	// illegal forward reference
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
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
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
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(12, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// FORREF1 BVS L19
	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(14, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 2;

	// LABEL3 JSR LABEL2
	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(16, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// FORREF2 JMP L21
	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(19, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// random instruction in order to test gauge symbol insertion
	// L2 STY $AA
	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(22, search_symbol(symtab, "L2"));
	pc += 2;

	// JMP (WHERE)
	buffer = ind_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(find_operand(lexer), instr, symtab));
	pc += 3;

	// FORREF3 JMP (WHERE)
	buffer = label_ind_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(27, search_symbol(symtab, "FORREF3"));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_parse_operand(void)
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

	buffer = imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_abs;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = label_imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = label_indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));

	// the earlier tests already defined ADDRESS
	// so we must reset the symbol table
	destroy_symbol_table(symtab);
	symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	// same test suite as test_parse_label_operand()
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);
	buffer = imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = "RANDOM_LABEL\t\tDEY\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, symtab));
	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_operand(lexer, instr, symtab));
	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_operand(lexer, instr, symtab));
	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	// pc is 0 in all 4 of these but it doesn't matter
	// we already tested symbols having correct pc in previous test suite
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_operand(lexer, instr, symtab));
	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_operand(lexer, instr, symtab));
	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_operand(lexer, instr, symtab));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_apply_masks(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int expected = 0;
	const char *buffer;

	buffer = imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// no need to parse
	expected = NOT_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));
	// 0x1FFF is 13 bits, which is all we ever use

	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected value is the same for these tests
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = X_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = Y_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = NOT_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = NOT_REGISTER_FIELD & NOT_INDIRECT_FIELD & ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = X_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	expected = Y_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// expected = ;
	TEST_ASSERT_EQUAL_INT(expected, apply_masks(lexer, 0x1FFF));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_forward_reference_addr_mode(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int expected = 0;
	const char *buffer;

	// don't insert any symbols

	buffer = label_branch_forref;
	expected = ADDR_MODE_RELATIVE & NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, instr->addr_bitflag);

	buffer = label_jump_forref;
	expected = ABSOLUTE_FIELD & NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, instr->addr_bitflag);

	buffer = ind_forref;
	expected = ABSOLUTE_FIELD & INDIRECT_FIELD & NOT_REGISTER_FIELD;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, instr->addr_bitflag);

	buffer = label_ind_forref;
	// same expected masks
	// expected = ;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, instr->addr_bitflag);

	// bad forward refs
	buffer = "BCC (LABEL)\n";
	expected = 0;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);
	buffer = "BVS (LABEL),Y\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);
	buffer = "JMP #LABEL\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);

	// parse_forward_reference_addr_mode() doesn't recognize JSR with indirect label as
	// an error
	// this is due to the limited masks in parse_forward_reference_addr_mode()
	// therefore this error handling will be redelegated to the code generator
	// buffer = "JSR (LABEL)\n";
	// TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	// TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode(lexer, instr));
	// TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);
	// TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_addr_mode(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	int operand_status;
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
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));

	buffer = imp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_IMPLIED, parse_addr_mode(operand_status, lexer, instr));

	// exhaustive_lines.h has exhaustive token sequences
	// BUT NOT exhaustive addressing modes (missing accumulator)
	buffer = "\t\tASL\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ACCUMULATOR, parse_addr_mode(operand_status, lexer, instr));

	buffer = "\t\tLSR\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ACCUMULATOR, parse_addr_mode(operand_status, lexer, instr));

	buffer = "\t\tROL\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ACCUMULATOR, parse_addr_mode(operand_status, lexer, instr));

	buffer = "\t\tROR\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ACCUMULATOR, parse_addr_mode(operand_status, lexer, instr));

	buffer = zp;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE, parse_addr_mode(operand_status, lexer, instr));

	buffer = zp_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE, parse_addr_mode(operand_status, lexer, instr));

	buffer = absolute;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, parse_addr_mode(operand_status, lexer, instr));

	buffer = abs_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, parse_addr_mode(operand_status, lexer, instr));

	buffer = zpx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE_X, parse_addr_mode(operand_status, lexer, instr));

	buffer = zpx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE_X, parse_addr_mode(operand_status, lexer, instr));

	buffer = absx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE_X, parse_addr_mode(operand_status, lexer, instr));

	buffer = absx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE_X, parse_addr_mode(operand_status, lexer, instr));

	buffer = zpy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE_Y, parse_addr_mode(operand_status, lexer, instr));

	buffer = zpy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ZERO_PAGE_Y, parse_addr_mode(operand_status, lexer, instr));

	buffer = absy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE_Y, parse_addr_mode(operand_status, lexer, instr));

	buffer = absy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE_Y, parse_addr_mode(operand_status, lexer, instr));

	buffer = ind;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, parse_addr_mode(operand_status, lexer, instr));

	buffer = ind_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, parse_addr_mode(operand_status, lexer, instr));

	buffer = imm;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_IMMEDIATE, parse_addr_mode(operand_status, lexer, instr));

	buffer = imm_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_IMMEDIATE, parse_addr_mode(operand_status, lexer, instr));

	buffer = indx;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_X_INDEXED_INDIRECT, parse_addr_mode(operand_status, lexer, instr));

	buffer = indx_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_X_INDEXED_INDIRECT, parse_addr_mode(operand_status, lexer, instr));

	buffer = indy;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT_Y_INDEXED, parse_addr_mode(operand_status, lexer, instr));

	buffer = indy_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT_Y_INDEXED, parse_addr_mode(operand_status, lexer, instr));

	buffer = branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, parse_addr_mode(operand_status, lexer, instr));

	buffer = branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));
	// no need to check results of parse_forward_reference_addr_mode()
	// we already tested that exhaustively

	buffer = jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, parse_addr_mode(operand_status, lexer, instr));

	buffer = jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));

	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	// actual pc value doesn't matter for these tests
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, parse_addr_mode(operand_status, lexer, instr));

	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));

	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, parse_addr_mode(operand_status, lexer, instr));

	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));

	buffer = ind_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));

	buffer = label_ind_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(operand_status, lexer, instr));

	// bad branches
	buffer = "BCC (ADDRESS)\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	buffer = "BCS (CONSTANT8,X)\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	buffer = "BVS (CONSTANT16),Y\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	// bad immediateS
	buffer = "JMP\t#000\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	buffer = "BMI #CONSTANT16\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	// bad jumps
	buffer = "\tJMP    (LABEL1),Y\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

	buffer = "\t\tJSR\t(ADDRESS,X)\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	operand_status = parse_operand(lexer, instr, symtab);
	TEST_ASSERT_EQUAL_INT(0, parse_addr_mode(operand_status, lexer, instr));

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
	RUN_TEST(test_parse_operand);
	RUN_TEST(test_apply_masks);
	RUN_TEST(test_parse_forward_reference_addr_mode);
	RUN_TEST(test_parse_addr_mode);

	return UNITY_END();
}