/** main.c

The Gaia assembler for 6502 Assembly.

*/

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "forward_reference.h"
#include "generator.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

const char *argp_program_version = "gaia assembler 1.0";

static char *args_doc = "SOURCE.asm";
static char *doc = "Gaia assembles a 6502 source code file into a binary file.";

// array of cmdline options
static struct argp_option options[] = {
	{"output", 'o', "FILE", 0, "Place assembled binary into <FILE>", 0},
	// {"cmos",   'c', NULL,   0, "Use 65C02 opcodes",                  0}
};

// struct used as storage for arg_parse()
struct Arguments {
	char *src_file;
	char *out_file;
	// int cmos;
};

/* parse_opt()
	Parse an option or argument from the command line.
*/
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct Arguments *args = state->input;

	switch (key) {
	// case 'c':
	// 	args->cmos = 1;
	// 	break;
	case 'o':
		args->out_file = arg;
		break;
	case ARGP_KEY_ARG:
		// arg_num starts at 0
		if (state->arg_num >= 1) {
			// too many arguments
			argp_usage(state);
		}
		args->src_file = arg;
		break;
	case ARGP_KEY_END:
		// arg_num is incremented after returning from ARGP_KEY_ARG
		// if arg_num is still its initial value of zero then we never
		// saw ARGP_KEY_ARG
		if (state->arg_num == 0)
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

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
	struct Arguments args = {.src_file = NULL, .out_file = "a.out"};//,
	                         // .cmos = 0};
	struct argp argp = {.options = options, .parser = parse_opt,
	                    .args_doc = args_doc, .doc = doc, .children = NULL,
	                    .help_filter = NULL, .argp_domain = NULL};
	argp_parse(&argp, argc, argv, 0, NULL, &args);

	FILE *inf = NULL;
	FILE *outf = NULL;
	struct Lexer *lexer = init_lexer(args.src_file);
	struct Token *tk = init_token();
	struct Instruction *instr = init_instruction();
	struct SymbolTable *symtab = init_symbol_table();
	struct Unresolved *unresolved = init_unresolved();
	if (!lexer || !tk || !instr || !symtab || !unresolved) {
		print_error(NULL, ERROR_MEMORY_ALLOCATION_FAIL, NULL,
		            args.src_file, -1);
		ABORT_ASSEMBLY();
	}

	inf = fopen(args.src_file, "r");
	if (!inf) {
		print_error(NULL, ERROR_FILE_OPEN_FAIL, NULL, args.src_file,
		            -1);
		ABORT_ASSEMBLY();
	}
	outf = fopen(args.out_file, "wb");
	if (!outf) {
		print_error(NULL, ERROR_BINARY_FILE_CREATION_FAIL, NULL,
		            args.src_file, -1);
		ABORT_ASSEMBLY();
	}

	char buffer[MAX_BUFFER_SIZE];
	int line_num = 1;
	int return_code;
	int pc = 0x0;
	struct Token *operand;
	struct Token *first;
	int operand_status;
	int written_bytes = 0;
	struct ForwardRef *ref;
	int total_bytes = 0;
	while (fgets(buffer, MAX_BUFFER_SIZE, inf)) {
		return_code = lex_line(buffer, lexer, tk, instr, line_num);
		if (IS_ERROR(return_code))
			ABORT_ASSEMBLY();

		return_code = parse_line(lexer);
		if (IS_ERROR(return_code))
			ABORT_ASSEMBLY();

		first = lexer->sequence[0];
		if (first->type == TOKEN_LABEL ||
		    first->type == TOKEN_DEFINE_DIRECTIVE) {
			return_code = parse_label_declaration(lexer, symtab, pc);
			if (IS_ERROR(return_code))
				ABORT_ASSEMBLY();
		} else if (first->type == TOKEN_BASE ||
		           first->type == TOKEN_ORG_DIRECTIVE) {
			pc = first->value;
		} else if (first->type == TOKEN_END_DIRECTIVE) {
			break;
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
		if (IS_ERROR(written_bytes))
			ABORT_ASSEMBLY();

		line_num++;
		pc += written_bytes;
		total_bytes += written_bytes;
	}

	int dest_pc;
	for (int i = 0; i < unresolved->curr; i++) {
		ref = unresolved->refs[i];
		dest_pc = search_symbol(symtab, ref->label);
		if (IS_ERROR(dest_pc)) {
			print_error(ref->source_line,
			            ERROR_MISSING_LABEL_DEFINITION,
			            ref->operand_location, args.src_file,
			            ref->line_num);
			ABORT_ASSEMBLY();
		}

		written_bytes = resolve_forward_ref(outf, ref, lexer, dest_pc);
		if (IS_ERROR(written_bytes))
			ABORT_ASSEMBLY();
	}

	printf("Assembly successful.\n");
	printf("%i (0x%04x) bytes assembled.\n", total_bytes, total_bytes);
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
