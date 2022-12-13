#include <stdio.h>

#include "../../unity/unity.h"
#include "error.h"
#include "symbol_table.h"

void setUp(void) {}
void tearDown(void) {}

void test_init_destroy_symtab(void)
{
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	TEST_ASSERT_EQUAL_INT(17, symtab->size);
	TEST_ASSERT_EQUAL_INT(0, symtab->count);
	for (int i = 0; i < symtab->size; i++)
		TEST_ASSERT_NULL(symtab->symbols[i]);

	destroy_symbol_table(symtab);
}

void test_insert_symbol(void)
{
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	TEST_ASSERT_EQUAL_INT(17, symtab->size);
	TEST_ASSERT_EQUAL_INT(0, symtab->count);
	// hash 8
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "JANUS", 1));
	TEST_ASSERT_EQUAL_INT(1, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[8]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[8]->label);
	TEST_ASSERT_EQUAL_STRING("JANUS", symtab->symbols[8]->label);
	// hash 15
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "CHKEND", 2));
	TEST_ASSERT_EQUAL_INT(2, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[15]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[15]->label);
	TEST_ASSERT_EQUAL_STRING("CHKEND", symtab->symbols[15]->label);
	// hash 15 again, should be probed into index 16
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "SORT8", 3));
	TEST_ASSERT_EQUAL_INT(3, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[16]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[16]->label);
	TEST_ASSERT_EQUAL_STRING("SORT8", symtab->symbols[16]->label);
	// hash 4
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "NXTEL", 4));
	TEST_ASSERT_EQUAL_INT(4, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[4]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[4]->label);
	TEST_ASSERT_EQUAL_STRING("NXTEL", symtab->symbols[4]->label);
	// hash 15 again, should be probed into index 0 (wrap around 16)
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "OPNING", 5));
	TEST_ASSERT_EQUAL_INT(5, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[0]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[0]->label);
	TEST_ASSERT_EQUAL_STRING("OPNING", symtab->symbols[0]->label);
	// hash 11
	TEST_ASSERT_EQUAL_INT(1, insert_symbol(symtab, "SWAP", 6));
	TEST_ASSERT_EQUAL_INT(6, symtab->count);
	TEST_ASSERT_NOT_NULL(symtab->symbols[11]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[11]->label);
	TEST_ASSERT_EQUAL_STRING("SWAP", symtab->symbols[11]->label);

	destroy_symbol_table(symtab);
}

void test_search_symbol(void)
{
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	insert_symbol(symtab, "JANUS", 1);
	insert_symbol(symtab, "CHKEND", 2);
	insert_symbol(symtab, "SORT8", 3);
	insert_symbol(symtab, "NXTEL", 4);
	insert_symbol(symtab, "OPNING", 5);
	insert_symbol(symtab, "SWAP", 6);

	TEST_ASSERT_EQUAL_INT(1, search_symbol(symtab, "JANUS"));
	TEST_ASSERT_EQUAL_INT(2, search_symbol(symtab, "CHKEND"));
	TEST_ASSERT_EQUAL_INT(3, search_symbol(symtab, "SORT8"));
	TEST_ASSERT_EQUAL_INT(4, search_symbol(symtab, "NXTEL"));
	TEST_ASSERT_EQUAL_INT(5, search_symbol(symtab, "OPNING"));
	TEST_ASSERT_EQUAL_INT(6, search_symbol(symtab, "SWAP"));

	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "lol"));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "main"));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "SORT16"));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "!@#$%^&*()"));
	TEST_ASSERT_EQUAL_INT(ERROR_SYMBOL_NOT_FOUND, search_symbol(symtab, "SETW"));

	// hash 12
	insert_symbol(symtab, "SORT16", 7);
	TEST_ASSERT_NOT_NULL(symtab->symbols[12]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[12]->label);
	TEST_ASSERT_EQUAL_INT(7, search_symbol(symtab, "SORT16"));

	// hash 15, probed into index 1 ("OPNING" is at index 0)
	insert_symbol(symtab, "SETW", 8);
	TEST_ASSERT_NOT_NULL(symtab->symbols[1]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[1]->label);
	TEST_ASSERT_EQUAL_INT(8, search_symbol(symtab, "SETW"));

	destroy_symbol_table(symtab);
}

// the resize function is supposed to be static
// can't test it anymore ;)
/*
// resize the symtab yourself
void test_resize(void)
{
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	TEST_ASSERT_EQUAL_INT(17, symtab->size);
	TEST_ASSERT_NOT_NULL(resize_symbols(symtab));
	TEST_ASSERT_EQUAL_INT(37, symtab->size);

	destroy_symbol_table(symtab);
}
*/

// symtab resizes itself after inserting over the load factor
void test_resize_automatic(void)
{
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	TEST_ASSERT_EQUAL_INT(17, symtab->size);
	// hash 8
	insert_symbol(symtab, "JANUS", 1);
	// hash 15
	insert_symbol(symtab, "CHKEND", 2);
	// hash 15 -> index 16
	insert_symbol(symtab, "SORT8", 3);
	// hash 4
	insert_symbol(symtab, "NXTEL", 4);
	// hash 15 -> index 0
	insert_symbol(symtab, "OPNING", 5);
	// hash 11
	insert_symbol(symtab, "SWAP", 6);
	// hash 12
	insert_symbol(symtab, "SORT16", 7);
	// hash 15 -> index 1
	insert_symbol(symtab, "SETW", 8);

	// insert checks if load factor > 0.5
	// 17 / 2 = 8.5 -> 8
	// we resize at the 9th element
	TEST_ASSERT_EQUAL_INT(17, symtab->size);
	insert_symbol(symtab, "REVERSE", 9);
	TEST_ASSERT_EQUAL_INT(37, symtab->size);

	// new hashes:
	// JANUS   20
	TEST_ASSERT_NOT_NULL(symtab->symbols[20]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[20]->label);
	TEST_ASSERT_EQUAL_STRING("JANUS", symtab->symbols[20]->label);
	// CHKEND  24
	TEST_ASSERT_NOT_NULL(symtab->symbols[24]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[24]->label);
	TEST_ASSERT_EQUAL_STRING("CHKEND", symtab->symbols[24]->label);
	// SORT8   26
	TEST_ASSERT_NOT_NULL(symtab->symbols[26]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[26]->label);
	TEST_ASSERT_EQUAL_STRING("SORT8", symtab->symbols[26]->label);
	// NXTEL   21
	TEST_ASSERT_NOT_NULL(symtab->symbols[21]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[21]->label);
	TEST_ASSERT_EQUAL_STRING("NXTEL", symtab->symbols[21]->label);
	// OPNING  19
	TEST_ASSERT_NOT_NULL(symtab->symbols[19]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[19]->label);
	TEST_ASSERT_EQUAL_STRING("OPNING", symtab->symbols[19]->label);
	// SWAP    33
	TEST_ASSERT_NOT_NULL(symtab->symbols[33]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[33]->label);
	TEST_ASSERT_EQUAL_STRING("SWAP", symtab->symbols[33]->label);
	// SORT16  15
	TEST_ASSERT_NOT_NULL(symtab->symbols[15]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[15]->label);
	TEST_ASSERT_EQUAL_STRING("SORT16", symtab->symbols[15]->label);
	// SETW    9
	TEST_ASSERT_NOT_NULL(symtab->symbols[9]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[9]->label);
	TEST_ASSERT_EQUAL_STRING("SETW", symtab->symbols[9]->label);
	// REVERSE 2
	TEST_ASSERT_NOT_NULL(symtab->symbols[2]);
	TEST_ASSERT_NOT_NULL(symtab->symbols[2]->label);
	TEST_ASSERT_EQUAL_STRING("REVERSE", symtab->symbols[2]->label);

	destroy_symbol_table(symtab);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_destroy_symtab);
	RUN_TEST(test_insert_symbol);
	RUN_TEST(test_search_symbol);
	// RUN_TEST(test_resize);
	RUN_TEST(test_resize_automatic);
	return UNITY_END();
}