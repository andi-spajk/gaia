/** error.c

Error-handling module for printing error messages. An arrow is also (not always)
printed, which is aligned to the error in the source line.

*/

#include <stdio.h>
#include <string.h>

#include "error.h"

/* print_error()
	@line           source line with the error
	@error_code     error code from error.h
	@bad_char       ptr to beginning of error in @line
	@file_name      name of assembly source file
	@line_num       line number of @line

	Prints a formatted error message and an arrow pointing to the error
	character.

	The following do not print source lines or the error arrow:
		ERROR_MEMORY_ALLOCATION_FAIL
		ERROR_FILE_OPEN_FAIL
		ERROR_BINARY_FILE_CREATION_FAIL
	These cases should pass in NULL for @line and @bad_char.
	The following do not print the error arrow:
		ERROR_UNKNOWN
		ERROR_ILLEGAL_ADDRESSING_MODE
		ERROR_TOO_BIG_OFFSET
*/
void print_error(const char *line, int error_code, const char *bad_char,
                 const char *file_name, int line_num)
{
	if (line)
		printf("%s:%d: ", file_name, line_num);

	switch (error_code) {
	case ERROR_MEMORY_ALLOCATION_FAIL:
		printf("ERROR: could not allocate enough memory during ");
		printf("assembly\n");
		break;
	case ERROR_ILLEGAL_CHAR:
		printf("ERROR: illegal character\n");
		break;
	case ERROR_TOO_BIG_LITERAL:
		printf("ERROR: value of literal exceeds 2 bytes\n");
		break;
	case ERROR_TOO_LONG_LABEL:
		printf("ERROR: label exceeds 63 characters\n");
		break;
	case ERROR_TOO_MANY_TOKENS:
		printf("ERROR: illegal sequence\n");
		break;
	case ERROR_UNKNOWN:
		printf("ERROR: unknown error while assembling\n");
		break;
	case ERROR_ILLEGAL_SEQUENCE:
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
		printf("ERROR: %s could not be opened\n", file_name);
		break;
	case ERROR_BINARY_FILE_CREATION_FAIL:
		printf("ERROR: assembled binary file could not be created\n");
		break;
	case ERROR_MISSING_LABEL_DEFINITION:
		printf("ERROR: label definition was not found\n");
		break;
	case ERROR_ILLEGAL_DIRECTIVE:
		printf("ERROR: illegal assembler directive\n");
		break;
	case ERROR_ILLEGAL_BASE:
		printf("ERROR: illegal base\n");
		break;
	default:
		return;
	}

	if (!line) {
		putchar('\n');
		return;
	}

	// skip trailing whitespace
	const char *end = line + strlen(line) - 1;
	while (*end == ' ' || *end == '\t' || *end == '\n' || *end == EOF)
		end--;
	for (const char *start = line; start <= end; start++)
		putchar(*start);
	putchar('\n');

	if (!bad_char) {
		putchar('\n');
		return;
	}

	int num_spaces = 0;
	int tab_align = 0;
	for (const char *tmp = line; tmp != bad_char; tmp++) {
		if (tab_align == 8)
			tab_align = 0;

		if (*tmp == '\t') {
			for (int i = 0; i < (8 - tab_align); i++)
				num_spaces++;
			tab_align = 0;
		} else {
			num_spaces++;
			tab_align++;
		}
	}

	for (int i = 0; i < num_spaces; i++)
		putchar(' ');
	// putchar('^');
	// for (const char *tmp = bad_char+1; !is_end_of_token(*tmp); tmp++) {
	// 	putchar('~');
	// }
	printf("^~~~~~\n\n");
}
