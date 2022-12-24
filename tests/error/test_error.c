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

//                           0123 4567890123
	const char *line = "\tLDA\t$800!!!!!!SDJGHJSDHFSDKVNSFULM\n";
	// pretend memory allocation error
	print_error(ERROR_MEMORY_ALLOCATION_FAIL, "prog.asm", 1, line);

	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_CHAR, lex_line(line, lexer, tk, instr));
	print_error(ERROR_ILLEGAL_CHAR, "prog.asm", 2, line);

	line = "TEST = $FFFF0001";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_LITERAL, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_BIG_LITERAL, "prog.asm", 3, line);

//               0 1234 56
	line = "\t\tJMP\ta2345678b2345678c2345678d2345678e2345678f2345678g2345678h2345678\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_LONG_LABEL, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_LONG_LABEL, "prog.asm", 5, line);

	line = "LABEL\t\tADC\t((PLACE,X),Y)\t;lolwtf\n";
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_MANY_TOKENS, lex_line(line, lexer, tk, instr));
	print_error(ERROR_TOO_MANY_TOKENS, "prog.asm", 6, line);

	print_error(ERROR_UNKNOWN, "prog.asm", 7, line);

	line = "INC     (LOL,),\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_SEQUENCE, parse_line(lexer));
	print_error(ERROR_ILLEGAL_SEQUENCE, "prog.asm", 8, line);

	line = "SYMBOL = $1234\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "SYMBOL = $0\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 0));
	print_error(ERROR_LABEL_REDEFINITION, "prog.asm", 9, line);

	line = "LOOP ADC $01\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, parse_label_declaration(lexer, symtab, 0));
	line = "LOOP JMP BYE\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_LABEL_REDEFINITION, parse_label_declaration(lexer, symtab, 2));
	TEST_ASSERT_EQUAL_INT(0, search_symbol(symtab, lexer->sequence[0]->str));
	print_error(ERROR_LABEL_REDEFINITION, "prog.asm", 10, line);

	line = "\t\tCPY\tADDRESS\n";
	TEST_ASSERT_EQUAL_INT(LEXER_SUCCESS, lex_line(line, lexer, tk, instr));
	TEST_ASSERT_EQUAL_INT(PARSER_SUCCESS, parse_line(lexer));
	TEST_ASSERT_EQUAL_INT(ERROR_ILLEGAL_FORWARD_REFERENCE, parse_operand(lexer, instr, symtab));
	print_error(ERROR_ILLEGAL_FORWARD_REFERENCE, "prog.asm", 10, line);

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