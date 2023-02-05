/** symbol_table.c

Symbol table for the assembler. Symbols consist of a label-value pair, with the
label acting as the key.

Initialize and destroy symbol tables, and insert and search symbols. No function
to delete symbol -- assembler doesn't need it.

This implementation uses djb2 hashing and linear probing. The starting size is
17 symbols. Resizing occurs with a load factor of 0.5. The new size is the next
prime number that is greater than double the previous size.

*/

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "prime.h"
#include "symbol_table.h"

// initial size of symbol table
#define BASE_SIZE 17

/* init_symbol_table()
	@return         ptr to dynamically allocated SymbolTable, or NULL
	                if fail

	Dynamically allocates a SymbolTable struct and its array of symbols.
*/
struct SymbolTable *init_symbol_table(void)
{
	struct SymbolTable *symtab = malloc(sizeof(struct SymbolTable));
	if (!symtab)
		return NULL;

	symtab->symbols = calloc((size_t)BASE_SIZE, sizeof(struct Symbol *));
	if (!symtab->symbols)
		return NULL;

	symtab->size = BASE_SIZE;
	symtab->count = 0;
	return symtab;
}

/* destroy_symbol_table()
	@symtab         ptr to SymbolTable struct

	Frees all the memory used by a SymbolTable and by its symbols.
*/
void destroy_symbol_table(struct SymbolTable *symtab)
{
	if (!symtab)
		return;
	if (!symtab->symbols)
		return;

	struct Symbol *curr;
	for (int i = 0; i < symtab->size; i++) {
		curr = symtab->symbols[i];
		if (curr) {
			if (curr->label)
				free(curr->label);
			free(curr);
		}
	}
	free(symtab->symbols);
	free(symtab);
}

/* djb2_hash()
	@label          string containing assembly label

	@return         32-bit hash value of the label

	Apply the djb2 hash algorithm to a string.
*/
static unsigned long djb2_hash(const char *label)
{
	unsigned long hash = 5381;
	for (int i = 0; label[i] != '\0'; i++)
		hash = ((hash << 5) + hash) + label[i];
	return hash;
}

/* resize_symbols()
	@symtab         ptr to SymbolTable struct

	@return         dynamically allocated array of ptrs to Symbol structs,
	                or NULL if fail

	Creates a new symbols[] array for a SymbolTable struct, rehashes all
	old items, and inserts them into the new symbols array. The memory
	of the old symbols array is freed.
*/
static struct Symbol **resize_symbols(struct SymbolTable *symtab)
{
	// double the old size, and find the next prime after that
	int new_size = next_prime(symtab->size * 2);
	struct Symbol **new_symbols = calloc((size_t)new_size,
	                                     sizeof(struct Symbol *));
	if (!new_symbols)
		return NULL;

	// rehash and transfer symbols from old array to new array
	struct Symbol *old_sym;
	struct Symbol *new_sym;
	int rehash;
	for (int i = 0; i < symtab->size; i++) {
		old_sym = symtab->symbols[i];
		if (old_sym) {
			rehash = djb2_hash(old_sym->label) % new_size;
			new_sym = new_symbols[rehash];
			// linearly probe for an open spot
			while (new_sym) {
				// wrap around to 0 if we reach end of array
				rehash = (rehash + 1) % new_size;
				new_sym = new_symbols[rehash];
			}
			new_symbols[rehash] = old_sym;
		}
	}
	free(symtab->symbols);
	symtab->symbols = new_symbols;
	symtab->size = new_size;
	return new_symbols;
}

/* init_symbol()
	@label          string containing assembly label
	@value          value of the label

	@return         ptr to dynamically allocated Symbol struct, or NULL if
	                failed to allocate

	Dynamically allocates a Symbol struct with a key-value pair of @label
	and @value. The Symbol's label is dynamically allocated too.
*/
static struct Symbol *init_symbol(const char *label, const int value)
{
	struct Symbol *sym = malloc(sizeof(struct Symbol));
	if (!sym)
		return NULL;

	sym->label = calloc(strlen(label) + 1, sizeof(char));
	if (!sym->label)
		return NULL;

	memcpy(sym->label, label, strlen(label));
	sym->value = value;
	return sym;
}

/* insert_symbol()
	@symtab         ptr to SymbolTable struct
	@label          string containing assembly label
	@value          value of the label

	@return         success, or error code

	Initialize, hash, and insert a symbol into the symbol table. Find open
	slots via linear probing. Calls resize if the load factor of 0.5 is
	reached.
*/
int insert_symbol(struct SymbolTable *symtab, const char *label,
                  const int value)
{
	// check load factor first
	// if you insert first then you might have to hash it again -- waste!
	int load = ((symtab->count + 1) * 100) / symtab->size;
	if (load > 50) {
		if (!resize_symbols(symtab))
			return ERROR_MEMORY_ALLOCATION_FAIL;
	}

	struct Symbol *new_sym = init_symbol(label, value);
	if (!new_sym)
		return ERROR_MEMORY_ALLOCATION_FAIL;

	// linearly probe for an open spot
	int hash = djb2_hash(label) % symtab->size;
	struct Symbol *curr = symtab->symbols[hash];
	while (curr) {
		// wrap around to index 0 if we reach end of array
		hash = (hash + 1) % symtab->size;
		curr = symtab->symbols[hash];
	}
	symtab->symbols[hash] = new_sym;
	symtab->count++;
	return SYMBOL_INSERTION_SUCCESS;
}

/* search_symbol()
	@symtab         ptr to SymbolTable struct
	@label          string containing assembly label

	@return         label's value is label found, or error code

	Searches the symbol table for @label and returns the label's value.
*/
int search_symbol(const struct SymbolTable *symtab, const char *label)
{
	int hash = djb2_hash(label) % symtab->size;
	struct Symbol *curr = symtab->symbols[hash];
	while (curr) {
		if (!strcmp(label, curr->label))
			return curr->value;
		hash = (hash + 1) % symtab->size;
		curr = symtab->symbols[hash];
	}
	// curr is now NULL, meaning the probing ended (or never started)
	return ERROR_SYMBOL_NOT_FOUND;
}
