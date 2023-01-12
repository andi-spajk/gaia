/** lexer.h

Prototypes and structs that perform lexical analysis and save tokens into a
Lexer struct. Types of tokens are represented as enums.

*/

#ifndef LEXER_H
#define LEXER_H

#include "opcode.h"

#define MAX_TOKENS 8

// success codes
#define LEXER_SUCCESS           1
#define TOKEN_INSERTION_SUCCESS 2

enum TokenType {
	TOKEN_LABEL,
	TOKEN_LITERAL,
	TOKEN_INSTRUCTION,
	TOKEN_IMMEDIATE,
	TOKEN_OPEN_PARENTHESIS,
	TOKEN_CLOSE_PARENTHESIS,
	TOKEN_COMMA,
	TOKEN_EQUAL_SIGN,
	TOKEN_X_REGISTER,
	TOKEN_Y_REGISTER,
	TOKEN_BASE,
	TOKEN_EQU_DIRECTIVE,
	TOKEN_DEFINE_DIRECTIVE,
	TOKEN_ORG_DIRECTIVE,
	TOKEN_END_DIRECTIVE,
	TOKEN_ACCUMULATOR,
	TOKEN_NULL
};

struct Token {
	enum TokenType type;
	char *str;
	unsigned int value;
	const char *error_char;       // location of lexical error
	const char *buffer_location;  // location of token in source line
};

struct Lexer {
	struct Token **sequence;
	int curr;                // first open index in sequence[]
	struct Token *error_tk;  // ptr to token with syntax error, only
	                         // determined by parser.c
	const char *line;        // ptr to original source line of code
	int line_num;
	const char *file_name;
};

struct Token *init_token(void);
int token_strcpy(struct Token *tk, const char *str);
int token_strncpy(struct Token *tk, const char *buffer, int length);
struct Lexer *init_lexer(const char *file_name);
void reset_lexer(struct Lexer *lexer);

void destroy_token(struct Token *tk);
void destroy_lexer(struct Lexer *lexer);

int add_token(struct Lexer *lexer, const struct Token *tk);

int lex_literal(const char *literal, struct Token *tk);
int lex_instruction(struct Token *tk, struct Instruction *instr);
int lex_directive(const char *buffer, struct Token *tk);
int lex_text(const char *buffer, struct Token *tk, struct Instruction *instr);
int lex(const char *buffer, struct Token *tk, struct Instruction *instr);
int lex_line(const char *buffer, struct Lexer *lexer, struct Token *tk,
             struct Instruction *instr, int line_num);

#endif