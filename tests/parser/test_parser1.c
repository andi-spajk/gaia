#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

#include "exhaustive_lines.h"

void setUp(void) {}
void tearDown(void) {}

#define PARSE_INSTR_TREE_TESTER(result_code, buffer, lexer, tk, instr, line_num, index) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT((result_code), parse_instr_tree((lexer), (index))); \
	(line_num)++; \
}

void test_parse_instr_tree(void)
{
	struct Lexer *lexer = init_lexer("test_parse_instr_tree.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int line_num = 1;

	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, imp, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zp, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zp_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, absolute, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, abs_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zpx, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zpx_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, absx, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, absx_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zpy, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, zpy_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, absy, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, absy_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, ind, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, ind_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, imm, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, imm_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, indx, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, indx_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, indy, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, indy_label, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, branch, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, branch_forref, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, jump, lexer, tk, instr, line_num, 0);
	PARSE_INSTR_TREE_TESTER(PARSER_SUCCESS, jump_forref, lexer, tk, instr, line_num, 0);

	const char *bad = "LDA LDX LDY LOLOL\n";
	PARSE_INSTR_TREE_TESTER(ERROR_ILLEGAL_SEQUENCE, bad, lexer, tk, instr, line_num, 0);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);

	const char *really_bad = "LDX (),\tX LABEL\n";
	PARSE_INSTR_TREE_TESTER(ERROR_ILLEGAL_SEQUENCE, really_bad, lexer, tk, instr, line_num, 0);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], lexer->error_tk);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

#define PARSE_LABEL_TREE_TESTER(result_code, buffer, lexer, tk, instr, line_num, index) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT((result_code), parse_label_tree((lexer), (index))); \
	(line_num)++; \
}

void test_parse_label_tree(void)
{
	struct Lexer *lexer = init_lexer("test_parse_label_tree.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int line_num = 1;

	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, constant_addr, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, constant_8bit, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, constant_16bit, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, equ_directive, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, lone_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_branch, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_branch_forref, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_jump, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_jump_forref, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_imp, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zp, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zp_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_abs, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_abs_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zpx, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zpx_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_absx, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_absx_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zpy, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_zpy_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_absy, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_absy_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_ind, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_ind_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_imm, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_imm_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_indx, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_indx_label, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_indy, lexer, tk, instr, line_num, 0);
	PARSE_LABEL_TREE_TESTER(PARSER_SUCCESS, label_indy_label, lexer, tk, instr, line_num, 0);

	const char *too_many = "LABEL1 LABEL2 STLOOP JANUS YOYOYOEOYEOYOEYO\n";
	PARSE_LABEL_TREE_TESTER(ERROR_ILLEGAL_SEQUENCE, too_many, lexer, tk, instr, line_num, 0);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);

	const char *triple = "sexy === $ffff\n";
	PARSE_LABEL_TREE_TESTER(ERROR_ILLEGAL_SEQUENCE, triple, lexer, tk, instr, line_num, 0);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], lexer->error_tk);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

#define PARSE_DIRECTIVE_TREE_TESTER(result_code, buffer, lexer, tk, instr, line_num, index) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT(result_code, parse_directive_tree((lexer), (index))); \
	(line_num)++; \
}

void test_parse_directive_tree(void)
{
	struct Lexer *lexer = init_lexer("test_parse_directive_tree.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int line_num = 1;

	PARSE_DIRECTIVE_TREE_TESTER(PARSER_SUCCESS, define_directive, lexer, tk, instr, line_num, 0);
	PARSE_DIRECTIVE_TREE_TESTER(PARSER_SUCCESS, org_directive, lexer, tk, instr, line_num, 0);
	PARSE_DIRECTIVE_TREE_TESTER(PARSER_SUCCESS, end_directive, lexer, tk, instr, line_num, 0);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_base_tree(void)
{
	struct Lexer *lexer = init_lexer("test_parse_base_tree.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int line_num = 1;

	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(base, lexer, tk, instr, line_num));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_base_tree(lexer, 0));
	line_num++;

	const char *buffer = "\t\t*=bad";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_base_tree(lexer, 0));
	line_num++;

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

#define SETUP_TESTER(result_code, buffer, lexer, tk, instr, line_num) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT((result_code), parse_line((lexer))); \
	(line_num)++; \
}

void test_parse_line(void)
{
	struct Lexer *lexer = init_lexer("test_parse_line.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	// const char *buffer;
	int line_num = 1;

	SETUP_TESTER(PARSER_SUCCESS, constant_addr, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, constant_8bit, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, constant_16bit, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, equ_directive, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, define_directive, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, org_directive, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, base, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, lone_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, imp, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zp, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zp_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, absolute, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, abs_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zpx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zpx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, absx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, absx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zpy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, zpy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, absy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, absy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, ind, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, ind_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, imm, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, imm_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, indx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, indx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, indy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, indy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, branch, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, branch_forref, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, jump, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, jump_forref, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_branch, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_branch_forref, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_jump, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_jump_forref, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_imp, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zp, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zp_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_abs, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_abs_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zpx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zpx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_absx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_absx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zpy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_zpy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_absy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_absy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_ind, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_ind_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_imm, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_imm_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_indx, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_indx_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_indy, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, label_indy_label, lexer, tk, instr, line_num);
	SETUP_TESTER(PARSER_SUCCESS, end_directive, lexer, tk, instr, line_num);

	const char *bad = "LDA LDX LDY LOLOL\n";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);
	const char *really_bad = "LDX (),\tX LABEL\n";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, really_bad, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], lexer->error_tk);
	const char *too_many = "LABEL1 LABEL2 STLOOP JANUS YOYOYOEOYEOYOEYO\n";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, too_many, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);
	const char *triple = "sexy === $ffff\n";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, triple, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[2], lexer->error_tk);
	const char *bad_label = "LDA X\n";  // parser reads X as the register
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);
	const char *bad_directive = "number .define $9999";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad_directive, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);
	const char *bad_define_directive = ".equ wow $666";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad_define_directive, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[0], lexer->error_tk);
	const char *bad_directive3 = ".END ZP,X";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad_directive3, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);
	const char *bad_directive4 = ".ORG CONSTANT16";
	SETUP_TESTER(ERROR_ILLEGAL_SEQUENCE, bad_directive4, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);

	const char *blank1 = "\n";
	SETUP_TESTER(PARSER_SUCCESS, blank1, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(NULL, lexer->error_tk);
	const char *blank2 = "\r\n";
	SETUP_TESTER(PARSER_SUCCESS, blank2, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(NULL, lexer->error_tk);
	const char *blank3 = "   \t\t  \t  \n";
	SETUP_TESTER(PARSER_SUCCESS, blank3, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(NULL, lexer->error_tk);
	const char *comment = ";lolololol\r\n";
	SETUP_TESTER(PARSER_SUCCESS, comment, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_PTR(NULL, lexer->error_tk);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

void test_parse_label_declaration(void)
{
	struct Lexer *lexer = init_lexer("test_parse_label_declaration.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	const char *buffer;
	int line_num = 1;

	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0xABCD, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0xABCD, lexer->sequence[0]->value);
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x7F, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x7F, lexer->sequence[0]->value);
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	TEST_ASSERT_EQUAL_INT(0x1111, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x1111, lexer->sequence[0]->value);
	// let's set pc = 2
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x2, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x2, lexer->sequence[0]->value);
	buffer = equ_directive;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x4000, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(0x4000, lexer->sequence[0]->value);
	buffer = define_directive;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x8000, search_symbol(symtab, lexer->sequence[1]->str));
	TEST_ASSERT_EQUAL_INT(0x8000, lexer->sequence[1]->value);

	const char *label_redefinition = "CONSTANT16\t\t=\t$1234\n";
	buffer = label_redefinition;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[0], lexer->error_tk);
	// old value should still be saved in symtab
	TEST_ASSERT_EQUAL_INT(0x1111, search_symbol(symtab, lexer->sequence[0]->str));

	const char *label_redefinition2 = "LABEL1          TXA\n";
	buffer = label_redefinition2;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0x2));
	TEST_ASSERT_EQUAL_INT(0x2, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[0], lexer->error_tk);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

#define FIND_OPERAND_TESTER(operand, buffer, lexer, tk, instr, line_num) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_PTR((operand), find_operand((lexer))); \
	(line_num)++; \
}

void test_find_operand(void)
{
	struct Lexer *lexer = init_lexer("test_find_operand.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	// const char *buffer;
	int line_num = 1;

	TEST_ASSERT_EQUAL_PTR(NULL, find_operand(NULL));

	FIND_OPERAND_TESTER(NULL, imp, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zp, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zp_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], absolute, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], abs_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zpx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zpx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], absx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], absx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zpy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], zpy_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], absy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[1], absy_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], ind, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], ind_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], imm, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], imm_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], indx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], indx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], indy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], indy_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(NULL, label_imp, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zp, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zp_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_abs, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_abs_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zpx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zpx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_absx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_absx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zpy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_zpy_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_absy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[2], label_absy_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_ind, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_ind_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_imm, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_imm_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_indx, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_indx_label, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_indy, lexer, tk, instr, line_num);
	FIND_OPERAND_TESTER(lexer->sequence[3], label_indy_label, lexer, tk, instr, line_num);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

// borrow SETUP_TESTER() macro

void test_parse_label_operand(void)
{
	struct Lexer *lexer = init_lexer("test_parse_label_operand.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	const char *buffer;
	int line_num = 1;

	// set up constant labels
	SETUP_TESTER(PARSER_SUCCESS, constant_addr, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	SETUP_TESTER(PARSER_SUCCESS, constant_8bit, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	SETUP_TESTER(PARSER_SUCCESS, constant_16bit, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));

	// label operands that are not branch nor jump
	SETUP_TESTER(PARSER_SUCCESS, zp_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, abs_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, zpx_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, absx_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, zpy_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, absy_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));

	// no indirect instruction because that is always a jump!

	SETUP_TESTER(PARSER_SUCCESS, imm_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, indx_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	SETUP_TESTER(PARSER_SUCCESS, indy_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));

	// test case-insensitivity
	buffer = "ADC constant8\n";
	SETUP_TESTER(PARSER_SUCCESS, buffer, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));

	buffer = "STX constant16\n";
	SETUP_TESTER(PARSER_SUCCESS, buffer, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));

	buffer = "\t\tSTY\taddress\n";
	SETUP_TESTER(PARSER_SUCCESS, buffer, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_operand(lexer, instr, find_operand(lexer), symtab));



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

	// reset line num
	line_num = 1;
	int pc = 0x0;

	// LABEL1
	SETUP_TESTER(PARSER_SUCCESS, lone_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(0x0, search_symbol(symtab, "LABEL1"));

	// INX
	SETUP_TESTER(PARSER_SUCCESS, imp, lexer, tk, instr, line_num);
	pc += 1;

	// illegal forward reference
	// CPY ADDRESS
	SETUP_TESTER(PARSER_SUCCESS, abs_label, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "ADDRESS"));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE,
	                      parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	TEST_ASSERT_EQUAL_PTR(lexer->sequence[1], lexer->error_tk);

	// RANDOM_LABEL DEY
	buffer = "RANDOM_LABEL\t\tDEY\n";
	SETUP_TESTER(PARSER_SUCCESS, buffer, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	pc += 1;

	// BMI LABEL1
	SETUP_TESTER(PARSER_SUCCESS, branch, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 2;

	// BVS L1
	SETUP_TESTER(PARSER_SUCCESS, branch_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 2;

	// JMP LABEL1
	SETUP_TESTER(PARSER_SUCCESS, jump, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 3;

	// JSR L3
	SETUP_TESTER(PARSER_SUCCESS, jump_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 3;

	// LABEL2 BCC LABEL1
	SETUP_TESTER(PARSER_SUCCESS, label_branch, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(12, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 2;

	// FORREF1 BVS L19
	SETUP_TESTER(PARSER_SUCCESS, label_branch_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(14, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 2;

	// LABEL3 JSR LABEL2
	SETUP_TESTER(PARSER_SUCCESS, label_jump, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(16, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 3;

	// FORREF2 JMP L21
	SETUP_TESTER(PARSER_SUCCESS, label_jump_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(19, search_symbol(symtab, lexer->sequence[0]->str));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 3;

	// random instruction in order to test gauge symbol insertion
	// L2 STY $AA
	SETUP_TESTER(PARSER_SUCCESS, label_zp, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(22, search_symbol(symtab, "L2"));
	pc += 2;

	// JMP (WHERE)
	SETUP_TESTER(PARSER_SUCCESS, ind_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));
	pc += 3;

	// FORREF3 JMP (WHERE)
	SETUP_TESTER(PARSER_SUCCESS, label_ind_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, pc));
	TEST_ASSERT_EQUAL_INT(27, search_symbol(symtab, "FORREF3"));
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_label_operand(lexer, instr, find_operand(lexer), symtab));

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
	RUN_TEST(test_parse_directive_tree);
	RUN_TEST(test_parse_base_tree);
	RUN_TEST(test_parse_line);
	RUN_TEST(test_parse_label_declaration);
	RUN_TEST(test_find_operand);
	RUN_TEST(test_parse_label_operand);

	return UNITY_END();
}