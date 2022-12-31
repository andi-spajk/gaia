/** error.c

Error-handling module for printing error messages.

*/

#include <stdio.h>

#include "error.h"

/* print_error()
	@error_code     error code from error.h
	@file_name      name of assembly program file
	@line_num       line number where the error occurred
	@line           line with the error

	Prints a formatted error message.

	Memory errors do not print lines and should pass in NULL for @line.
	@line_num will be ignored in such cases.
*/
void print_error(int error_code, const char *file_name, int line_num,
                 const char *line)
{
	if (line)
		printf("%s:%i: ", file_name, line_num);
	else
		printf("%s: ", file_name);

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
		printf("ERROR: invalid token sequence\n");
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
	}

	if (line)
		printf("%s\n", line);
}
