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
	// int line_num = 1;

	// pretend
	print_error(NULL, ERROR_MEMORY_ALLOCATION_FAIL, NULL);

//                           0123 4567890123
	const char *line = "\tLDA\t$800!!!!!!SDJGHJSDHFSDKVNSFULM\n";
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_line(line, lexer, tk, instr));
	// line_num++;

	line = "LDX\t%10021010\n";
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_line(line, lexer, tk, instr));
	// line_num++;

	line = "TEST = $FFFF0001\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_line(line, lexer, tk, instr));
	// line_num++;

//               0 1234 56
	line = "\t\tJMP\ta2345678b2345678c2345678d2345678e2345678f2345678g2345678h2345678\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_LONG_LABEL, lex_line(line, lexer, tk, instr));
	// line_num++;

	line = "LABEL\t\tADC\t((PLACE,X),Y)\t;lolwtf\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_MANY_TOKENS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	// line_num++;

	// pretend
	print_error(line, ERROR_UNKNOWN, NULL);
	// line_num++;

	line = "INC     (LOL,),\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	// line_num++;

	line = "SYMBOL = $1234\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "SYMBOL = $0\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0));
	// line_num++;

	line = "\t\tLOOP\tADC\t$01\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "\t\tLOOP\tJMP\tBYE\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 2));
	TEST_ASSERT_EQUAL_INT(0, search_symbol(symtab, lexer->sequence[0]->str));
	// line_num++;

	line = "\t\tCPY\tADDRESS\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE, parse_operand(lexer, instr, find_operand(lexer), symtab));
	// line_num++;

	line = "SBC ($ABCD)\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_operand(lexer, instr, find_operand(lexer), symtab));
	int addr_mask = parse_addr_mode(lexer, instr, find_operand(lexer), PARSER_SUCCESS);
	instr->addr_bitflag = addr_mask & instr->addr_bitfield;
	TEST_ASSERT_EQUAL_INT(0, instr->addr_bitflag);
	// line_num++;

//              01234
	line = "BCC BACK\n";
	// test_generator.c already knows that the error code and error_tk will be set correctly
	// too lazy to repeat that here
	print_error(line, ERROR_TOO_BIG_OFFSET, &(line[4]));
	// line_num++;

	print_error(NULL, ERROR_FILE_OPEN_FAIL, NULL);
	print_error(NULL, ERROR_BINARY_FILE_CREATION_FAIL, NULL);

	// should do nothing
	print_error(line, -999, &(line[0]));

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