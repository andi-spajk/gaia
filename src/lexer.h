#ifndef LEXER_H
#define LEXER_H

enum TokenType {
	TOKEN_LABEL,
	TOKEN_LITERAL,
	TOKEN_INSTRUCTION,
	TOKEN_EQUAL_SIGN,
	TOKEN_OPEN_PARENTHESIS,
	TOKEN_CLOSE_PARENTHESIS,
	TOKEN_COMMA,
	TOKEN_X_REGISTER,
	TOKEN_Y_REGISTER,
	TOKEN_NULL
};

struct Token {
	enum TokenType type;
	char *str;
	unsigned int *value;
};

struct Lexer {
	struct Token **sequence;
	int curr;
};

struct Lexer *init_lexer(void);
void destroy_lexer(struct Lexer *lexer);

#endif