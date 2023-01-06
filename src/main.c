/** main.c

The Gaia assembler for 6502 Assembly.

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

// don't put semi-colon otherwise we can't write `ABORT_ASSEMBLY();`
#define ABORT_ASSEMBLY() return free_gaia(inf, outf, lexer, tk, instr, symtab, unresolved)

#define MAX_BUFFER_SIZE 128

/* free_gaia()
	@inf            source file
	@outf           assembled binary file
	@lexer          ptr to Lexer
	@tk             running token
	@instr          ptr to Instruction struct
	@symtab         symbol table
	@unresolved     ptr to Unresolved struct

	@return         EXIT_FAILURE code

	Close all files and destroy all allocated data structures being used
	by the Gaia assembler.
*/
int free_gaia(FILE *inf, FILE *outf, struct Lexer *lexer, struct Token *tk,
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
	return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
	// no argument parsing yet
	if (argc != 2) {
		printf("ERROR: invalid command-line arguments\n\n");
		return EXIT_FAILURE;
	}

	char *src_file = argv[1];
	if (!src_file) {
		printf("ERROR: no source file\n\n");
		return EXIT_FAILURE;
	}

	FILE *inf = NULL;
	FILE *outf = NULL;
	struct Lexer *lexer = init_lexer(src_file);
	struct Token *tk = init_token();
	struct Instruction *instr = init_instruction();
	struct SymbolTable *symtab = init_symbol_table();
	struct Unresolved *unresolved = init_unresolved();
	if (!lexer || !tk || !instr || !symtab || !unresolved) {
		print_error(NULL, ERROR_MEMORY_ALLOCATION_FAIL, NULL, src_file,
		            -1);
		ABORT_ASSEMBLY();
	}
	char *bin_file = "a.out";

	inf = fopen(src_file, "r");
	if (!inf) {
		print_error(NULL, ERROR_FILE_OPEN_FAIL, NULL, src_file, -1);
		ABORT_ASSEMBLY();
	}
	outf = fopen(bin_file, "wb");
	if (!outf) {
		print_error(NULL, ERROR_BINARY_FILE_CREATION_FAIL, NULL,
		            src_file, -1);
		ABORT_ASSEMBLY();
	}

	char buffer[MAX_BUFFER_SIZE];
	int line_num = 1;
	int error_code;
	int pc = 0x0;
	struct Token *operand = NULL;
	int operand_status;
	int written_bytes = 0;
	while (fgets(buffer, MAX_BUFFER_SIZE, inf)) {
		error_code = lex_line(buffer, lexer, tk, instr, line_num);
		if (error_code < 0)
			ABORT_ASSEMBLY();

		error_code = parse_line(lexer);
		if (error_code < 0)
			ABORT_ASSEMBLY();

		if (lexer->sequence[0]->type == TOKEN_LABEL) {
			error_code = parse_label_declaration(lexer, symtab, pc);
			if (error_code < 0)
				ABORT_ASSEMBLY();
		}

		if (instr->mnemonic != NULL_MNEMONIC) {
			operand = find_operand(lexer);
			operand_status = parse_operand(lexer, instr, operand,
			                               symtab);
			if (operand_status < 0)
				ABORT_ASSEMBLY();
			else if (operand_status == BRANCH_FORWARD_REFERENCE)
				written_bytes = 2;
			else if (operand_status == JUMP_FORWARD_REFERENCE)
				written_bytes = 3;
		}

		printf("%03i\t%s", line_num, buffer);
		line_num++;
	}
	printf("\n\n");
	if (written_bytes == -99999)
		printf("shut up compiler");

	struct Symbol *sym;
	for (int i = 0; i < symtab->size; i++) {
		sym = symtab->symbols[i];
		if (sym)
			printf("%02i: %s -- %i\n", i, sym->label, sym->value);
	}

	fclose(inf);
	fclose(outf);
	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	destroy_unresolved(unresolved);
	return 0;
}
