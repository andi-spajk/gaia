/** lexer.c

Lexical analyzer for the assembler. Scans a line of source code and divides it
into tokens. The tokens and their associated meanings are saved into a sequence
of tokens. Instruction tokens are analyzed and saved into a separate Instruction
struct. The sequence and instruction struct are used by the future parser.

The lexer detects lexical errors such as illegal characters and labels over the
maximum character length. It also checks if literal operands are over 2 bytes,
which is always an error.
*/

#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define MAX_TOKENS 8
#define MAX_TOKEN_STR_LEN 32

/* init_token()
	@return         ptr to dynamically allocated Token struct

	Dynamically allocates a Token struct and initializes its members.
	The Token's str is also dynamically allocated.
*/
struct Token *init_token(void)
{
	struct Token *token = malloc(sizeof(struct Token));
	if (!token)
		return NULL;

	token->type = TOKEN_NULL;
	token->str = calloc((size_t)MAX_TOKEN_STR_LEN, sizeof(char));
	if (!token->str)
		return NULL;
	token->value = 0U;
	return token;
}

/* init_sequence()
	@return         ptr to dynamically allocated array of ptrs to Token
	                structs, or NULL if fail

	Dynamically allocates an array of Token pointers and initializes
	the Tokens in the array.
*/
static struct Token **init_sequence(void)
{
	struct Token **sequence = malloc(sizeof(struct Token *) * MAX_TOKENS);
	if (!sequence)
		return NULL;

	// initialize tokens in the sequence
	for (int i = 0; i < MAX_TOKENS; i++) {
		sequence[i] = init_token();
		if (!sequence[i])
			return NULL;
	}
	return sequence;
}

/* init_lexer()
	@return         ptr to dynamically allocated Lexer, or NULL if fail

	Dynamically allocates a Lexer struct and its array of tokens.
*/
struct Lexer *init_lexer(void)
{
	struct Lexer *lexer = malloc(sizeof(struct Lexer));
	if (!lexer)
		return NULL;

	lexer->sequence = init_sequence();
	if (!lexer->sequence)
		return NULL;

	lexer->curr = 0;
	return lexer;
}

/* destroy_token()
	@tk             ptr to dynamically allocated Token struct

	Frees the memory used by a dynamically allocated Token and by its
	members.
*/
void destroy_token(struct Token *tk)
{
	if (tk) {
		if (tk->str)
			free(tk->str);
		free(tk);
	}
}

/* destroy_lexer()
	@lexer         ptr to Lexer struct

	Frees the memory used by a dynamically allcoated Lexer and by its
	members.
*/
void destroy_lexer(struct Lexer *lexer)
{
	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		destroy_token(curr);
	}
	free(lexer->sequence);
	free(lexer);
}

struct Token *add_token(struct Lexer *lexer, struct Token *tk)
{
	// end of sequence array
	if (lexer->curr == MAX_TOKENS)
		return NULL;

	struct Token *curr = lexer->sequence[lexer->curr];
	if (curr->type == TOKEN_NULL) {
		curr->type = tk->type;
		strncpy(curr->str, tk->str, MAX_TOKEN_STR_LEN);
		curr->value = tk->value;
		lexer->curr++;
		return curr;
	}
	return NULL;
}