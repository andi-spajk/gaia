#include "../../unity/unity.h"
#include "lexer.h"

// lexer.c has the same define, but it's not in lexer.h because the other
// modules don't need it
// therefore I must redefine NUM_TOKENS here just for this test file
#define NUM_TOKENS 8

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_lexer(void)
{
	struct Lexer *lexer = init_lexer();
	for (int i = 0; i < NUM_TOKENS; i++) {
		TEST_ASSERT_NOT_NULL(lexer->sequence[i]);

		TEST_ASSERT_EQUAL_INT(TOKEN_NULL, lexer->sequence[i]->type);

		TEST_ASSERT_NOT_NULL(lexer->sequence[i]->str);
		TEST_ASSERT_EQUAL_STRING("", lexer->sequence[i]->str);

		TEST_ASSERT_NOT_NULL(lexer->sequence[i]->value);
	}
	destroy_lexer(lexer);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_lexer);

	return UNITY_END();
}