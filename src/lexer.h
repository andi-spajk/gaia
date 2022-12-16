#ifndef LEXER_H
#define LEXER_H

#include "opcode.h"

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
	int curr;
};

struct Token *init_token(void);
struct Token *init_token_str(struct Token *tk, const char *str);
struct Lexer *init_lexer(void);

void destroy_token(struct Token *tk);
void destroy_lexer(struct Lexer *lexer);

struct Token *add_token(struct Lexer *lexer, const struct Token *tk);

int lex_literal(struct Token *tk, const char *line);
int lex_instruction(struct Token *tk, struct Instruction *instr);
int lex_text(const char *buffer, struct Token *tk, struct Instruction *instr);
int lex(const char *buffer, struct Token *tk, struct Instruction *instr);

#endif