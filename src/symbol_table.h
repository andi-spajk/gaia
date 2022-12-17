#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SYMBOL_INSERTION_SUCCESS 1

struct Symbol {
	// label and value form the key-value pair respectively
	char *label;
	int value;
};

struct SymbolTable {
	struct Symbol **symbols;
	int size;
	int count;
};

struct SymbolTable *init_symbol_table(void);
void destroy_symbol_table(struct SymbolTable *symtab);
int insert_symbol(struct SymbolTable *symtab, const char *label,
                  const int value);
int search_symbol(const struct SymbolTable *symtab, const char *label);

#endif