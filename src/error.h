/** error.h

Error codes used by the assembler. Also a prototype to print error messages.

*/

#ifndef ERROR_H
#define ERROR_H

// all codes are negative
#define ERROR_MEMORY_ALLOCATION_FAIL    -1

// internal error; no error message
#define ERROR_SYMBOL_NOT_FOUND          -2

#define ERROR_ILLEGAL_CHAR              -3
#define ERROR_TOO_BIG_LITERAL           -4

// internal error; no error message
#define ERROR_INSTRUCTION_NOT_FOUND     -6

#define ERROR_BITFIELD_NOT_FOUND        -7
#define ERROR_TOO_LONG_LABEL            -8
#define ERROR_TOO_MANY_TOKENS           -9
#define ERROR_UNKNOWN                   -10
#define ERROR_ILLEGAL_SEQUENCE          -11
#define ERROR_LABEL_REDEFINITION        -12
#define ERROR_ILLEGAL_FORWARD_REFERENCE -13
#define ERROR_ILLEGAL_ADDRESSING_MODE   -14

// internal error; should never happen
#define ERROR_ILLEGAL_MNEMONIC          -15

#define ERROR_TOO_BIG_OFFSET            -16
#define ERROR_FILE_OPEN_FAIL            -17
#define ERROR_BINARY_FILE_CREATION_FAIL -18
/*
;       ;       ;       ;       ;       ;
*/

void print_error(const char *line, int error_code, const char *bad_char);

#endif