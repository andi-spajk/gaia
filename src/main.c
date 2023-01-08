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

/** ABORT_ASSEMBLY()
	Free all data structures that were initialized, and close any FILEs that
	were opened. Then return the EXIT_FAILURE code.

	Call this macro in main() to end the assembling process and terminate
	the program. ONLY call this macro AFTER the FILEs and data structures
	have all been initialized, either successfully or unsuccessfully.

	The `do {} while(0)` syntax lets us write a semicolon, as in:
	`if (condition)`
		`ABORT_ASSEMBLY();`
	Otherwise, a multiline macro in a brace-less if-block would break the
	Linux kernel style:
	`if (condition)`
		`ABORT_ASSEMBLY();`
	`else`
		`something();`
	This solution was taken from section 3.10.3 of:
	https://www.acrc.bris.ac.uk/acrc/RedHat/rhel-cpp-en-4/macro-pitfalls.html
*/
#define ABORT_ASSEMBLY()                                                     \
	do {                                                                 \
		abort_gaia(inf, outf, lexer, tk, instr, symtab, unresolved); \
		return EXIT_FAILURE;                                         \
	} while (0)

/** IS_ERROR()
	@code           the return value of a function

	Check if a return code indicates an error occurred. All errors generate
	negative return codes.
*/
#define IS_ERROR(code) ((code) < 0)

#define MAX_BUFFER_SIZE 128

/* abort_gaia()
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
int abort_gaia(FILE *inf, FILE *outf, struct Lexer *lexer, struct Token *tk,
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
	// no robust argument parsing yet
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
	int return_code;
	int pc = 0x0;
	struct Token *operand = NULL;
	int operand_status;
	int written_bytes = 0;
	struct ForwardRef *ref;
	while (fgets(buffer, MAX_BUFFER_SIZE, inf)) {
		return_code = lex_line(buffer, lexer, tk, instr, line_num);
		if (IS_ERROR(return_code))
			ABORT_ASSEMBLY();

		return_code = parse_line(lexer);
		if (IS_ERROR(return_code))
			ABORT_ASSEMBLY();

		if (lexer->sequence[0]->type == TOKEN_LABEL) {
			return_code = parse_label_declaration(lexer, symtab, pc);
			if (IS_ERROR(return_code))
				ABORT_ASSEMBLY();
		}

		if (instr->mnemonic == NULL_MNEMONIC) {
			line_num++;
			continue;
		}

		operand = find_operand(lexer);
		operand_status = parse_operand(lexer, instr, operand,
		                               symtab);
		if (IS_ERROR(operand_status))
			ABORT_ASSEMBLY();
		else if (operand_status == BRANCH_FORWARD_REFERENCE)
			written_bytes = 2;
		else if (operand_status == JUMP_FORWARD_REFERENCE)
			written_bytes = 3;

		return_code = parse_addr_mode(lexer, instr, operand,
		                             operand_status);
		if (return_code == FORWARD_REFERENCE) {
			ref = create_forward_ref(buffer, instr, operand,
			                         operand_status, pc,
			                         line_num);
			if (!ref)
				ABORT_ASSEMBLY();
			if (IS_ERROR(add_forward_ref(unresolved, ref)))
				ABORT_ASSEMBLY();

			// write dummy bytes
			fputc(0x00, outf);
			fputc(0x00, outf);
			if (written_bytes == 3)
				fputc(0x00, outf);
		} else if (IS_ERROR(return_code)) {
			ABORT_ASSEMBLY();
		}

		if (operand_status == BRANCH_OPERAND ||
		    operand_status == JUMP_OPERAND) {
			written_bytes = resolve_label_ref(outf, lexer, instr,
			                                  operand,
			                                  operand_status,
			                                  symtab, pc);
		} else {
			written_bytes = generate_code(outf, instr, operand, pc);
		}

		line_num++;
		pc += written_bytes;
	}

	int dest_pc;
	for (int i = 0; i < unresolved->curr; i++) {
		ref = unresolved->refs[i];
		dest_pc = search_symbol(symtab, ref->label);
		if (IS_ERROR(dest_pc)) {
			print_error(ref->source_line,
			            ERROR_MISSING_LABEL_DEFINITION,
			            ref->operand_location, src_file,
			            ref->line_num);
			ABORT_ASSEMBLY();
		}
		resolve_forward_ref(outf, ref, lexer, dest_pc);
	}

	printf("Assembly successful.\n");
	printf("%i (0x%04x) bytes assembled.\n", pc+1, pc+1);
	printf("%i symbols.\n", symtab->count);

	fclose(inf);
	fclose(outf);
	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	destroy_unresolved(unresolved);
	return 0;
}
