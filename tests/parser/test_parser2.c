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

// no more passing in a result code for parse_line()
// we did that in test_parser1.c
#define SETUP_TESTER(buffer, lexer, tk, instr, line_num) { \
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line((lexer))); \
	(line_num)++; \
}

#define PARSE_OPERAND_TESTER(operand_status, operand, buffer, lexer, tk, instr, line_num, symtab) { \
	SETUP_TESTER((buffer), (lexer), (tk), (instr), (line_num)); \
	(operand) = find_operand((lexer)); \
	TEST_ASSERT_EQUAL_INT((operand_status), parse_operand((lexer), (instr), (operand), (symtab))); \
}

void test_parse_operand(void)
{
	struct Lexer *lexer = init_lexer("test_parse_operand.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	struct Token *operand;
	int line_num = 1;

	// set up constant labels
	SETUP_TESTER(constant_addr, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	SETUP_TESTER(constant_8bit, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	SETUP_TESTER(constant_16bit, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));

	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, imp, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zp, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zp_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, absolute, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, abs_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zpx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zpx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, absx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, absx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zpy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, zpy_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, absy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, absy_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, ind, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(JUMP_OPERAND, operand, ind_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, imm, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, imm_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, indx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, indx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, indy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, indy_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_imp, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zp, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zp_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_abs, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_abs_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zpx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zpx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_absx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_absx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zpy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_zpy_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_absy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_absy_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_ind, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(JUMP_OPERAND, operand, label_ind_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_imm, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_imm_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_indx, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_indx_label, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_indy, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, label_indy_label, lexer, tk, instr, line_num, symtab);

	// the earlier tests already defined ADDRESS
	// so we must reset the symbol table
	destroy_symbol_table(symtab);
	symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	// same test suite as test_parse_label_operand()
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, lone_label, lexer, tk, instr, line_num, symtab);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);

	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, imp, lexer, tk, instr, line_num, symtab);

	const char *buffer = "RANDOM_LABEL\t\tDEY\n";
	PARSE_OPERAND_TESTER(PARSER_SUCCESS, operand, buffer, lexer, tk, instr, line_num, symtab);

	PARSE_OPERAND_TESTER(BRANCH_OPERAND, operand, branch, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(BRANCH_FORWARD_REFERENCE, operand, branch_forref, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(JUMP_OPERAND, operand, jump, lexer, tk, instr, line_num, symtab);
	PARSE_OPERAND_TESTER(JUMP_FORWARD_REFERENCE, operand, jump_forref, lexer, tk, instr, line_num, symtab);

	SETUP_TESTER(label_branch, lexer, tk, instr, line_num);
	// pc is 0 in all 4 of these but it doesn't matter
	// we already tested symbols having correct pc in previous test suite
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	TEST_ASSERT_EQUAL_INT(BRANCH_OPERAND, parse_operand(lexer, instr, operand, symtab));

	SETUP_TESTER(label_branch_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, parse_operand(lexer, instr, operand, symtab));

	SETUP_TESTER(label_jump, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	TEST_ASSERT_EQUAL_INT(JUMP_OPERAND, parse_operand(lexer, instr, operand, symtab));

	SETUP_TESTER(label_jump_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, parse_operand(lexer, instr, operand, symtab));

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

// no need to parse the line
#define APPLY_MASKS_TESTER(expected, buffer, lexer, tk, instr, line_num) {\
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT(expected, apply_masks((lexer), 0x1FFF)); \
	(line_num)++; \
}

void test_apply_masks(void)
{
	struct Lexer *lexer = init_lexer("test_apply_masks.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int expected = 0;
	int line_num = 1;

	// don't pass the exhaustive_lines.h directly into APPLY_MASKS_TESTER()
	// harder to keep track of when/why we update expected
	expected = NOT_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, imp, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, zp, lexer, tk, instr, line_num);

	// expected value is still the same for the following tests
	// will only update when necessary
	APPLY_MASKS_TESTER(expected, zp_label, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, absolute, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, abs_label, lexer, tk, instr, line_num);

	expected = X_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, zpx, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, zpx_label, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, absx, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, absx_label, lexer, tk, instr, line_num);

	expected = Y_REGISTER_FIELD & NOT_INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, zpy, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, zpy_label, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, absy, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, absy_label, lexer, tk, instr, line_num);

	expected = NOT_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, ind, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, ind_label, lexer, tk, instr, line_num);

	expected = NOT_REGISTER_FIELD & NOT_INDIRECT_FIELD & ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, imm, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, imm_label, lexer, tk, instr, line_num);

	expected = X_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, indx, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, indx_label, lexer, tk, instr, line_num);

	expected = Y_REGISTER_FIELD & INDIRECT_FIELD & ~ADDR_MODE_IMMEDIATE;
	APPLY_MASKS_TESTER(expected, indy, lexer, tk, instr, line_num);
	APPLY_MASKS_TESTER(expected, indy_label, lexer, tk, instr, line_num);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

#define FOR_REF_ADDR_TESTER(expected, buffer, lexer, tk, instr, line_num) {\
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line((buffer), (lexer), (tk), (instr), (line_num))); \
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_forward_reference_addr_mode((lexer), (instr))); \
	TEST_ASSERT_EQUAL_INT((expected), instr->addr_bitflag); \
	(line_num)++; \
}

void test_parse_forward_reference_addr_mode(void)
{
	struct Lexer *lexer = init_lexer("test_parse_forward_reference_addr_mode.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	int expected = 0;
	const char *buffer;
	int line_num = 1;

	// don't insert any symbols

	expected = ADDR_MODE_RELATIVE & NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD;
	FOR_REF_ADDR_TESTER(expected, label_branch_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, instr->addr_bitflag);

	expected = ABSOLUTE_FIELD & NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD;
	FOR_REF_ADDR_TESTER(expected, label_jump_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, instr->addr_bitflag);

	expected = ABSOLUTE_FIELD & INDIRECT_FIELD & NOT_REGISTER_FIELD;
	FOR_REF_ADDR_TESTER(expected, ind_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, instr->addr_bitflag);

	// same expected masks
	FOR_REF_ADDR_TESTER(expected, label_ind_forref, lexer, tk, instr, line_num);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_INDIRECT, instr->addr_bitflag);

	// bad forward refs
	buffer = "BCC (LABEL)\n";
	expected = 0;
	FOR_REF_ADDR_TESTER(expected, buffer, lexer, tk, instr, line_num);
	buffer = "BVS (LABEL),Y\n";
	FOR_REF_ADDR_TESTER(expected, buffer, lexer, tk, instr, line_num);
	buffer = "JMP #LABEL\n";
	FOR_REF_ADDR_TESTER(expected, buffer, lexer, tk, instr, line_num);
	buffer = "JSR (LABEL)\n";
	FOR_REF_ADDR_TESTER(expected, buffer, lexer, tk, instr, line_num);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
}

#define PARSE_ADDR_MODE_TESTER(expected, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num) { \
	SETUP_TESTER((buffer), (lexer), (tk), (instr), (line_num)); \
	(operand) = find_operand((lexer)); \
	(operand_status) = parse_operand((lexer), (instr), (operand), (symtab)); \
	TEST_ASSERT_EQUAL_INT((expected), parse_addr_mode((lexer), (instr), (operand), (operand_status))); \
}

void test_parse_addr_mode(void)
{
	struct Lexer *lexer = init_lexer("test_parse_addr_mode.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	int operand_status;
	const char *buffer;
	struct Token *operand;
	int line_num = 1;

	// set up constant labels
	buffer = constant_addr;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	buffer = constant_8bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	buffer = constant_16bit;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));
	buffer = lone_label;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_label_declaration(lexer, symtab, 0x0));

	PARSE_ADDR_MODE_TESTER(ADDR_MODE_IMPLIED, operand, operand_status, imp, lexer, tk, instr, symtab, line_num);

	// exhaustive_lines.h has exhaustive token sequences
	// BUT NOT exhaustive addressing modes (missing accumulator)
	buffer = "\t\tASL\n";
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ACCUMULATOR, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "\t\tLSR\n";
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ACCUMULATOR, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "\t\tROL\n";
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ACCUMULATOR, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "\t\tROR\n";
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ACCUMULATOR, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE, operand, operand_status, zp, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE, operand, operand_status, zp_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE, operand, operand_status, absolute, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE, operand, operand_status, abs_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE_X, operand, operand_status, zpx, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE_X, operand, operand_status, zpx_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE_X, operand, operand_status, absx, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE_X, operand, operand_status, absx_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE_Y, operand, operand_status, zpy, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ZERO_PAGE_Y, operand, operand_status, zpy_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE_Y, operand, operand_status, absy, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE_Y, operand, operand_status, absy_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_INDIRECT, operand, operand_status, ind, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_INDIRECT, operand, operand_status, ind_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_IMMEDIATE, operand, operand_status, imm, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_IMMEDIATE, operand, operand_status, imm_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_X_INDEXED_INDIRECT, operand, operand_status, indx, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_X_INDEXED_INDIRECT, operand, operand_status, indx_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_INDIRECT_Y_INDEXED, operand, operand_status, indy, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_INDIRECT_Y_INDEXED, operand, operand_status, indy_label, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_RELATIVE, operand, operand_status, branch, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(FORWARD_REFERENCE, operand, operand_status, branch_forref, lexer, tk, instr, symtab, line_num);
	// no need to check results of parse_forward_reference_addr_mode()
	// we already tested that exhaustively
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_ABSOLUTE, operand, operand_status, jump, lexer, tk, instr, symtab, line_num);
	PARSE_ADDR_MODE_TESTER(FORWARD_REFERENCE, operand, operand_status, jump_forref, lexer, tk, instr, symtab, line_num);

	buffer = label_branch;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	// actual pc value doesn't matter for these tests
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, parse_addr_mode(lexer, instr, operand, operand_status));

	buffer = label_branch_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	buffer = label_jump;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, parse_addr_mode(lexer, instr, operand, operand_status));

	buffer = label_jump_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	PARSE_ADDR_MODE_TESTER(FORWARD_REFERENCE, operand, operand_status, ind_forref, lexer, tk, instr, symtab, line_num);

	buffer = label_ind_forref;
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr, line_num++));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	// bad branches
	buffer = "BCC (ADDRESS)\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "BCS (CONSTANT8,X)\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "BVS (CONSTANT16),Y\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	// bad immediates
	buffer = "JMP\t#000\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "BMI #CONSTANT16\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	// bad jumps
	buffer = "\tJMP    (LABEL1),Y\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "\t\tJSR\t(ADDRESS,X)\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	buffer = "JSR (ADDRESS)\n";
	PARSE_ADDR_MODE_TESTER(0, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);

	// no operand
	buffer = "\t\tSTA\t;lol\n";
	// parse_addr_mode() assumes it's implied
	PARSE_ADDR_MODE_TESTER(ADDR_MODE_IMPLIED, operand, operand_status, buffer, lexer, tk, instr, symtab, line_num);
	// and we will check that it's an error
	TEST_ASSERT_EQUAL_INT(0, ADDR_MODE_IMPLIED & instr->addr_bitfield);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_parse_operand);
	RUN_TEST(test_apply_masks);
	RUN_TEST(test_parse_forward_reference_addr_mode);
	RUN_TEST(test_parse_addr_mode);

	return UNITY_END();
}