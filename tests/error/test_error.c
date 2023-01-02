#include "../../unity/unity.h"
#include "error.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"

void setUp(void) {}
void tearDown(void) {}

void test_print_error(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	int line_num = 1;

	// we can only pretend for these errors
	// line number is ignored in these cases
	print_error(ERROR_MEMORY_ALLOCATION_FAIL, "prog.asm", 999999, NULL);
	print_error(ERROR_INVALID_CMDLINE_ARGS, NULL, 12345, NULL);
	print_error(ERROR_BINARY_FILE_CREATION_FAIL, "prog.asm", -1, NULL);

//                           0123 4567890123
	const char *line = "\tLDA\t$800!!!!!!SDJGHJSDHFSDKVNSFULM\n";
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_line(line, lexer, tk, instr));
	print_error(ERROR_ILLEGAL_CHAR, "prog.asm", line_num, line);
	line_num++;

	line = "LDX\t%10021010\n";
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_line(line, lexer, tk, instr));
	print_error(ERROR_ILLEGAL_CHAR, "prog.asm", line_num, line);
	line_num++;

	line = "TEST = $FFFF0001\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_BIG_LITERAL, "prog.asm", line_num, line);
	line_num++;

//               0 1234 56
	line = "\t\tJMP\ta2345678b2345678c2345678d2345678e2345678f2345678g2345678h2345678\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_LONG_LABEL, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_LONG_LABEL, "prog.asm", line_num, line);
	line_num++;

	line = "LABEL\t\tADC\t((PLACE,X),Y)\t;lolwtf\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_MANY_TOKENS, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_MANY_TOKENS, "prog.asm", line_num, line);
	line_num++;

	print_error(ERROR_UNKNOWN, "prog.asm", line_num, line);
	line_num++;

	line = "INC     (LOL,),\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	print_error(ERROR_ILLEGAL_SEQUENCE, "prog.asm", line_num, line);
	line_num++;

	line = "SYMBOL = $1234\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "SYMBOL = $0\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0));
	print_error(ERROR_LABEL_REDEFINITION, "prog.asm", line_num, line);
	line_num++;

	line = "LOOP ADC $01\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "LOOP JMP BYE\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 2));
	TEST_ASSERT_EQUAL_INT(0, search_symbol(symtab, lexer->sequence[0]->str));
	print_error(ERROR_LABEL_REDEFINITION, "prog.asm", line_num, line);
	line_num++;

	line = "\t\tCPY\tADDRESS\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE, parse_operand(instr, find_operand(lexer), symtab));
	print_error(ERROR_ILLEGAL_FORWARD_REFERENCE, "prog.asm", line_num, line);
	line_num++;

	line = "SBC ($ABCD)\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(instr, find_operand(lexer), symtab));
	int addr_mask = parse_addr_mode(lexer, instr, find_operand(lexer), PARSER_SUCCESS);
	instr->addr_bitflag = addr_mask & instr->addr_bitfield;
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);
	print_error(ERROR_ILLEGAL_ADDRESSING_MODE, "prog.asm", line_num, line);
	line_num++;

	line = "BCC BACK\n";
	// test_generator.c already knows that the error code is returned correctly
	// too lazy to repeat the test here
	print_error(ERROR_TOO_BIG_OFFSET, "prog.asm", line_num, line);
	line_num++;

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_print_error);

	return UNITY_END();
}