#include "../../unity/unity.h"
#include "prime.h"

void setUp(void) {}
void tearDown(void) {}

// is_prime() should be static
// can't test anymore ;)
/*
void test_is_prime(void)
{
	TEST_ASSERT_FALSE(is_prime(1));
	TEST_ASSERT_TRUE(is_prime(2));
	TEST_ASSERT_TRUE(is_prime(3));
	TEST_ASSERT_FALSE(is_prime(4));
	TEST_ASSERT_TRUE(is_prime(5));
	TEST_ASSERT_FALSE(is_prime(6));
	TEST_ASSERT_TRUE(is_prime(7));
	TEST_ASSERT_FALSE(is_prime(51));
	TEST_ASSERT_FALSE(is_prime(10));
	TEST_ASSERT_FALSE(is_prime(12));
	TEST_ASSERT_TRUE(is_prime(223));
	TEST_ASSERT_TRUE(is_prime(227));
}
*/
void test_next_prime(void)
{
	TEST_ASSERT_EQUAL_INT(2, next_prime(1));
	TEST_ASSERT_EQUAL_INT(3, next_prime(2));
	TEST_ASSERT_EQUAL_INT(5, next_prime(3));
	TEST_ASSERT_EQUAL_INT(5, next_prime(4));
	TEST_ASSERT_EQUAL_INT(7, next_prime(5));
	TEST_ASSERT_EQUAL_INT(7, next_prime(6));
	TEST_ASSERT_EQUAL_INT(11, next_prime(7));
	TEST_ASSERT_EQUAL_INT(53, next_prime(51));
	TEST_ASSERT_EQUAL_INT(11, next_prime(10));
	TEST_ASSERT_EQUAL_INT(13, next_prime(12));
	TEST_ASSERT_EQUAL_INT(227, next_prime(223));
	TEST_ASSERT_EQUAL_INT(229, next_prime(227));

	TEST_ASSERT_EQUAL_INT(37, next_prime(17 * 2));
	TEST_ASSERT_EQUAL_INT(79, next_prime(37 * 2));
	TEST_ASSERT_EQUAL_INT(163, next_prime(79 * 2));
}

int main(void)
{
	UNITY_BEGIN();
	// RUN_TEST(test_is_prime);
	RUN_TEST(test_next_prime);
	return UNITY_END();
}