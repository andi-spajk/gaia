/** error.c

Error-handling module for printing error messages.

*/

#include <stdio.h>
#include <string.h>

#include "error.h"

/* print_error()
	@line           source line with the error
	@error_code     error code from error.h
	@bad_char       ptr to beginning of error

	Prints a formatted error message.

	Memory errors, file open erros, and invalid cmdline args do not print
	source lines. Both cases should pass in NULL for @line, and the latter
	should pass in NULL for @file_name. @line_num will be ignored in such
	cases.

	removed, maybe add back later:
	@file_name      name of assembly program file
	@line_num       line number where the error occurred

*/
void print_error(const char *line, int error_code, const char *bad_char)
{
	switch (error_code) {
	case ERROR_MEMORY_ALLOCATION_FAIL:
		printf("ERROR: could not allocate enough memory during assembly\n");
		printf("\n");
		return;
	// case ERROR_SYMBOL_NOT_FOUND:
	// 	break;
	case ERROR_ILLEGAL_CHAR:
		printf("ERROR: illegal character\n");
		break;
	case ERROR_TOO_BIG_LITERAL:
		printf("ERROR: value of literal exceeds 2 bytes\n");
		break;
	// case ERROR_TOO_MANY_INSTRUCTIONS:
	// 	printf("ERROR: invalid token sequence\n");
	// 	break;
	case ERROR_BITFIELD_NOT_FOUND:
		printf("ERROR: bad mnemonic\n");
		break;
	case ERROR_TOO_LONG_LABEL:
		printf("ERROR: label exceeds 63 characters\n");
		break;
	case ERROR_TOO_MANY_TOKENS:
		printf("ERROR: illegal sequence\n");
		break;
	case ERROR_UNKNOWN:
		printf("ERROR: unknown\n");
		break;
	case ERROR_ILLEGAL_SEQUENCE:
		// happens in parser.c so it must be syntax issue
		printf("ERROR: illegal syntax\n");
		break;
	case ERROR_LABEL_REDEFINITION:
		printf("ERROR: no redefinition of labels\n");
		break;
	case ERROR_ILLEGAL_FORWARD_REFERENCE:
		printf("ERROR: illegal forward reference\n");
		break;
	case ERROR_ILLEGAL_ADDRESSING_MODE:
		printf("ERROR: illegal addressing mode\n");
		break;
	case ERROR_TOO_BIG_OFFSET:
		printf("ERROR: branch offset too far away\n");
		break;
	case ERROR_FILE_OPEN_FAIL:
		printf("ERROR: program file could not be opened\n");
		break;
	case ERROR_BINARY_FILE_CREATION_FAIL:
		printf("ERROR: assembled binary file could not be created\n");
		printf("\n");
		break;
	}

	if (!line || !bad_char)
		return;

	// skip trailing whitespace
	const char *end = line + strlen(line) - 1;
	while (*end == ' ' || *end == '\t' || *end == '\n' || *end == EOF)
		end--;
	for (const char *start = line; start <= end; start++)
		putchar(*start);
	printf("\n");

	if (error_code == ERROR_UNKNOWN) {
		printf("\n");
		return;
	}

	int num_spaces = 0;
	int tab_tracker = 0;
	for (const char *tmp = line; tmp != bad_char; tmp++) {
		if (tab_tracker == 8) {
			tab_tracker = 0;
		}
		if (*tmp == '\t') {
			for (int i = 0; i < (8 - tab_tracker); i++)
				num_spaces++;
			tab_tracker = 0;
		} else {
			num_spaces++;
			tab_tracker++;
		}
	}

	for (int i = 0; i < num_spaces; i++)
		printf(" ");
	printf("^~~~~~\n\n");
}
