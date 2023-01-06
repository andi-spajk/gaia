/** main.c

6502 assembler.

*/

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "forward_reference.h"
#include "generator.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

#define ABORT_ASSEMBLY() { \
	end_assembly(inf, outf, lexer, tk, instr, symtab, unresolved); \
	return EXIT_FAILURE; \
}

#define MAX_BUFFER_SIZE 128

/* end_assembly()
	@inf            source file
	@outf           assembled binary file
	@lexer          ptr to Lexer
	@tk             running token
	@instr          ptr to Instruction struct
	@symtab         symbol table
	@unresolved     ptr to Unresolved struct

	Close all files and destroy all allocated data structures.
*/
void end_assembly(FILE *inf, FILE *outf, struct Lexer *lexer, struct Token *tk,
                  struct Instruction *instr, struct SymbolTable *symtab,
                  struct Unresolved *unresolved)
{
	if (inf)
		fclose(inf);
	if (outf)
		fclose(outf);
	if (lexer)
		destroy_lexer(lexer);
	if (tk)
		destroy_token(tk);
	if (instr)
		destroy_instruction(instr);
	if (symtab)
		destroy_symbol_table(symtab);
	if (unresolved)
		destroy_unresolved(unresolved);
}

int main(int argc, char *argv[])
{
	// no argument parsing yet
	if (argc != 2) {
		printf("ERROR: invalid command-line arguments\n");
		return EXIT_FAILURE;
	}

	char *src_file = argv[1];
	if (!src_file) {
		printf("ERROR: no source file\n");
		return EXIT_FAILURE;
	}

	struct Lexer *lexer = init_lexer(src_file);
	struct Token *tk = init_token();
	struct Instruction *instr = init_instruction();
	struct SymbolTable *symtab = init_symbol_table();
	struct Unresolved *unresolved = init_unresolved();
	if (!lexer || !tk || !instr || !symtab || !unresolved) {
		print_error(NULL, ERROR_MEMORY_ALLOCATION_FAIL, NULL, src_file,
		            -1);
		return EXIT_FAILURE;
	}
	char *bin_file = "a.out";
	FILE *inf, *outf;

	inf = fopen(src_file, "r");
	if (!inf) {
		print_error(NULL, ERROR_FILE_OPEN_FAIL, NULL, src_file, -1);
		end_assembly(inf, NULL, lexer, tk, instr, symtab, unresolved);
		return EXIT_FAILURE;
	}
	outf = fopen(bin_file, "wb");
	if (!outf) {
		print_error(NULL, ERROR_BINARY_FILE_CREATION_FAIL, NULL,
		            src_file, -1);
		end_assembly(inf, outf, lexer, tk, instr, symtab, unresolved);
		return EXIT_FAILURE;
	}

	char buffer[MAX_BUFFER_SIZE];
	int line_num = 1;
	// int pc = 0x0;
	int error_code;
	while (fgets(buffer, MAX_BUFFER_SIZE, inf)) {
		error_code = lex_line(buffer, lexer, tk, instr, line_num);
		if (error_code < 0)
			ABORT_ASSEMBLY();
		error_code = parse_line(lexer);
		if (error_code < 0)
			ABORT_ASSEMBLY();

		printf("%03i\t%s", line_num, buffer);
		line_num++;
	}
	putchar('\n');

	for (int i = 0; i < unresolved->size; i++) {
		if (unresolved->refs[i])
			printf("%s\n", unresolved->refs[i]->label);
	}

	end_assembly(inf, outf, lexer, tk, instr, symtab, unresolved);
	return 0;
}
