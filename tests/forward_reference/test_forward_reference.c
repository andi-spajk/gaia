#include "../../unity/unity.h"
#include "addressing_modes.h"
#include "bitfields.h"
#include "error.h"
#include "forward_reference.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_unresolved(void)
{
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);
	TEST_ASSERT_EQUAL_INT(0, unresolved->curr);
	TEST_ASSERT_EQUAL_INT(8, unresolved->size);
	destroy_unresolved(unresolved);
	destroy_unresolved(NULL);
}

void test_create_forward_ref(void)
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
	int pc;
	int line_num;
	int operand_status;
	struct ForwardRef *ref;
	struct Token *operand;

	TEST_ASSERT_NULL(create_forward_ref(NULL, NULL, NULL, JUMP_FORWARD_REFERENCE, 999, 999));

	// JMP L21
	buffer = "\t\tJMP\tL21\t; comment\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc = 0x0;
	line_num = 1;

	TEST_ASSERT_NULL(create_forward_ref(buffer, instr, NULL, JUMP_FORWARD_REFERENCE, pc, line_num));

	operand = find_operand(lexer);
	operand_status = parse_operand(instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(operand_status, JUMP_FORWARD_REFERENCE);

	int expected = NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD & ABSOLUTE_FIELD;
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);

	TEST_ASSERT_EQUAL_INT(0x0, ref->pc);
	TEST_ASSERT_EQUAL_INT(1, ref->line_num);
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, ref->operand_status);

	TEST_ASSERT_EQUAL_INT(JMP, ref->instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(JMP_BITFIELD, ref->instr->addr_bitfield);
	TEST_ASSERT_EQUAL_INT(expected, ref->instr->addr_bitflag);

	TEST_ASSERT_EQUAL_STRING("\t\tJMP\tL21\t; comment", ref->source_line);
	TEST_ASSERT_EQUAL_STRING("L21", ref->label);
	destroy_forward_ref(ref);

	// BNE LABEL1
	buffer = "             BNE LABEL1 \t\t\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc = 0x3;
	line_num = 2;

	operand = find_operand(lexer);
	operand_status = parse_operand(instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(operand_status, BRANCH_FORWARD_REFERENCE);

	expected = NOT_INDIRECT_FIELD & NOT_REGISTER_FIELD & ADDR_MODE_RELATIVE;
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	TEST_ASSERT_EQUAL_INT(expected, instr->addr_bitflag);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);

	TEST_ASSERT_EQUAL_INT(0x3, ref->pc);
	TEST_ASSERT_EQUAL_INT(2, ref->line_num);
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, ref->operand_status);

	TEST_ASSERT_EQUAL_INT(BNE, ref->instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(BNE_BITFIELD, ref->instr->addr_bitfield);
	TEST_ASSERT_EQUAL_INT(expected, ref->instr->addr_bitflag);

	TEST_ASSERT_EQUAL_STRING("             BNE LABEL1", ref->source_line);
	TEST_ASSERT_EQUAL_STRING("LABEL1", ref->label);

	destroy_forward_ref(ref);
	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_add_forward_ref(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);
	struct ForwardRef *ref;
	int pc;
	int line_num;
	int operand_status;
	const char *buffer;
	struct Token *operand;

	// JSR FORREF
	buffer = "\t\tJSR\tFORREF\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc = 0x0;
	line_num = 1;

	operand = find_operand(lexer);
	operand_status = parse_operand(instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(ERROR_NULL_ARGUMENT, add_forward_ref(NULL, ref));
	TEST_ASSERT_EQUAL_INT(ERROR_NULL_ARGUMENT, add_forward_ref(unresolved, NULL));

	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));

	ref = unresolved->refs[0];
	TEST_ASSERT_EQUAL_STRING("FORREF", ref->label);
	TEST_ASSERT_EQUAL_STRING("\t\tJSR\tFORREF", ref->source_line);
	TEST_ASSERT_EQUAL_INT(JSR, ref->instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_ABSOLUTE, ref->instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(0, ref->pc);
	TEST_ASSERT_EQUAL_INT(1, ref->line_num);
	TEST_ASSERT_EQUAL_INT(JUMP_FORWARD_REFERENCE, ref->operand_status);

	// BMI FORREF2
	buffer = "             BMI        FORREF2\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc = 0x3;
	line_num = 2;

	operand = find_operand(lexer);
	operand_status = parse_operand(instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	// we can reassign ref without freeing the previous ref because we
	// saved the previous ptr into unresolved->refs.
	// destroy_unresolved() will free that memory for us

	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));

	ref = unresolved->refs[1];
	TEST_ASSERT_EQUAL_STRING("FORREF2", ref->label);
	TEST_ASSERT_EQUAL_STRING("             BMI        FORREF2", ref->source_line);
	TEST_ASSERT_EQUAL_INT(BMI, ref->instr->mnemonic);
	TEST_ASSERT_EQUAL_INT(ADDR_MODE_RELATIVE, ref->instr->addr_bitflag);
	TEST_ASSERT_EQUAL_INT(3, ref->pc);
	TEST_ASSERT_EQUAL_INT(2, ref->line_num);
	TEST_ASSERT_EQUAL_INT(BRANCH_FORWARD_REFERENCE, ref->operand_status);

	destroy_unresolved(unresolved);
	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

void test_resize_unresolved(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);
	struct ForwardRef *ref;
	int pc;
	int line_num;
	int operand_status;
	const char *buffer;
	struct Token *operand;

	// JMP (FORREF)
	buffer = "\t\tJMP\t(FORREF)\t;hi\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(buffer, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	pc = 0x0;
	line_num = 1;

	operand = find_operand(lexer);
	operand_status = parse_operand(instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	TEST_ASSERT_EQUAL_INT(8, unresolved->size);
	TEST_ASSERT_EQUAL_INT(7, unresolved->curr);

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	TEST_ASSERT_EQUAL_INT(16, unresolved->size);
	TEST_ASSERT_EQUAL_INT(8, unresolved->curr);

	destroy_unresolved(unresolved);
	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_unresolved);
	RUN_TEST(test_create_forward_ref);
	RUN_TEST(test_add_forward_ref);
	RUN_TEST(test_resize_unresolved);

	return UNITY_END();
}