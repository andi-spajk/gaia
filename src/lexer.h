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
	TOKEN_NULL
};

struct Token {
	enum TokenType type;
	char *str;
	unsigned int value;
};

struct Lexer {
	struct Token **sequence;
	int curr;  // first open slot in sequence[]
};

struct Token *init_token(void);
int token_strcpy(struct Token *tk, const char *str);
int token_strncpy(struct Token *tk, const char *buffer, int length);
struct Lexer *init_lexer(void);
void reset_lexer(struct Lexer *lexer);

void destroy_token(struct Token *tk);
void destroy_lexer(struct Lexer *lexer);

int add_token(struct Lexer *lexer, const struct Token *tk);

int lex_literal(struct Token *tk, const char *literal);
int lex_instruction(struct Token *tk, struct Instruction *instr);
int lex_text(const char *buffer, struct Token *tk, struct Instruction *instr);
int lex(const char *buffer, struct Token *tk, struct Instruction *instr);
int lex_line(const char *buffer, struct Lexer *lexer, struct Token *tk,
             struct Instruction *instr);

#endif