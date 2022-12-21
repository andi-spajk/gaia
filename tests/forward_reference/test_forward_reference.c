#include "../../unity/unity.h"
#include "forward_reference.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_unresolved(void)
{
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);
	TEST_ASSERT_EQUAL_INT(0, unresolved->curr);
	TEST_ASSERT_EQUAL_INT(8, unresolved->size);
	destroy_unresolved(unresolved);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_init_destroy_unresolved);

	return UNITY_END();
}