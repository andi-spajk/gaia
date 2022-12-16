#ifndef ERROR_H
#define ERROR_H

// error codes
// all codes are negative
#define ERROR_MEMORY_ALLOCATION_FAIL    -1
#define ERROR_SYMBOL_NOT_FOUND          -2
#define ERROR_ILLEGAL_CHAR              -3
#define ERROR_TOO_BIG_LITERAL           -4
#define ERROR_TOO_MANY_INSTRUCTIONS     -5
#define ERROR_INSTRUCTION_NOT_FOUND     -6  // internal error; no error message
#define ERROR_BITFIELD_NOT_FOUND        -7
#define ERROR_TOO_LONG_LABEL            -8
#define ERROR_TOO_MANY_TOKENS           -9
/*
;       ;       ;       ;       ;       ;
*/
// success codes
#define LEXER_SUCCESS   1

#endif