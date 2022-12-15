/** lexer.c

Lexical analyzer for the assembler. Scans a line of source code and divides it
into tokens. The tokens and their associated meanings are saved into a sequence
of tokens. Instruction tokens are analyzed and saved into a separate Instruction
struct. The sequence and instruction struct are used by the future parser.

The lexer detects lexical errors such as illegal characters and labels over the
maximum character length. It also checks if literal operands are over 2 bytes,
which is always an error.

*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "lexer.h"
#include "opcode.h"

#define MAX_TOKENS 8
#define MAX_TOKEN_STR_LEN 64

/* init_token()
	@return         ptr to dynamically allocated Token struct

	Dynamically allocates a Token struct and initializes its members.
*/
struct Token *init_token(void)
{
	struct Token *tk = malloc(sizeof(struct Token));
	if (!tk)
		return NULL;

	tk->type = TOKEN_NULL;
	tk->str = NULL;
	tk->value = 0U;
	return tk;
}

/* init_token_str()
	@tk             ptr to Token struct
	@str            string to duplicate into @tk. Can be empty string

	@return         ptr to Token if @str is successfully duplicated, or
	                NULL if fail.

	Dynamically allocates the Token's string member. If @str is empty,
	nothing happens. Otherwise, @str will be duplicated into the token
	string.
*/
struct Token *init_token_str(struct Token *tk, char *str)
{
	if (!str)
		return tk;

	// include null terminator
	size_t length = strlen(str) + 1;
	if (!tk->str) {
		tk->str = calloc(length, sizeof(char));
	} else {
		// tk->str already contains a string
		// so this function call must be replacing the string
		tk->str = realloc(tk->str, length);
	}

	if (!tk->str)
		return NULL;

	// writing length bytes will always end with a null terminator in order
	// to cut off the chars that may be left over from prior strings
	strncpy(tk->str, str, length);
	return tk;
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

/* add_token()
	@lexer          ptr to Lexer struct
	@tk             ptr to Token struct with populated members

	@return         ptr to Token in the lexer sequence if success, or NULL
	                if fail

	Places @tk into the lexer sequence by copying @tk's data into the
	sequence's token. Token is placed in the last free position, if
	able.
*/
struct Token *add_token(struct Lexer *lexer, const struct Token *tk)
{
	// end of sequence array
	if (lexer->curr == MAX_TOKENS)
		return NULL;

	struct Token *curr = lexer->sequence[lexer->curr];
	if (curr->type == TOKEN_NULL) {
		curr->type = tk->type;
		if (!init_token_str(curr, tk->str))
			return NULL;
		curr->value = tk->value;
		lexer->curr++;
		return curr;
	}
	return NULL;
}

static int hex_digit_to_int(const char c)
{
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
		return c- 'a' + 10;
	else if (c >= '0' && c <= '9')
		return c - '0';
	return ERROR_ILLEGAL_CHAR;
}

static int bin_digit_to_int(const char c)
{
	if (c == '0')
		return 0;
	else if (c == '1')
		return 1;
	return ERROR_ILLEGAL_CHAR;
}

static int dec_digit_to_int(const char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	return ERROR_ILLEGAL_CHAR;
}

/* lex_literal()
	@tk             ptr to Token in the lexer sequence
	@line           ptr to first char of the literal token

	@return         number of chars read, or error code

	Lexically analyze a literal token and update @tk to hold the literal's
	information.
*/
int lex_literal(struct Token *tk, char *line)
{
	int (*converters[])(char) = {hex_digit_to_int, bin_digit_to_int,
	                             dec_digit_to_int};

	char *curr = line;
	int base;
	int converter_func;  // for indexing array of function ptrs
	if (*curr == '$') {
		base = 16;
		converter_func = 0;
		curr++;
	} else if (*curr == '%') {
		base = 2;
		converter_func = 1;
		curr++;
	} else if (*curr >= '0' && *curr <= '9') {
		base = 10;
		converter_func = 2;
	} else {
		return ERROR_ILLEGAL_CHAR;
	}

	// analyze all the digits and convert to actual value
	unsigned long total = 0;
	int num_chars = 0;
	int char_value = (*converters[converter_func])(*curr);
	while (curr && char_value != ERROR_ILLEGAL_CHAR) {
		total *= base;
		total += char_value;
		num_chars++;
		curr++;
		char_value = (*converters[converter_func])(*curr);
	}

	// no valid chars were found
	if (num_chars == 0)
		return ERROR_ILLEGAL_CHAR;
	if (total > 0xFFFF)
		return ERROR_TOO_BIG_LITERAL;

	tk->type = TOKEN_LITERAL;
	tk->value = (unsigned int)total;
	return num_chars;
}

/* lex_instruction()
	@tk             ptr to Token struct
	@instr          ptr to Instruction struct

	@return         number of chars read, or error code

	Lexically analyze @tk's string member and check if it's an instruction
	mnemonic. @instr and @tk are NOT modified by this function if a
	mnemonic was not found.
*/
int lex_instruction(struct Token *tk, struct Instruction *instr)
{
	enum Mnemonic mnemonic = str_to_mnemonic(tk->str);
	if (mnemonic != ILLEGAL_MNEMONIC) {
		tk->type = TOKEN_INSTRUCTION;
		instr->mnemonic = mnemonic;
		instr->addr_bitfield = get_addr_bitfield(mnemonic);
		return strlen(tk->str);
	}
	return ERROR_INSTRUCTION_NOT_FOUND;
}

// while scanning a token, encountering one of these characters means that a
// new token is next
static int is_end_of_token(const char c)
{
	switch (c) {
	case ' ':
	case '\t':
	case '\n':
	case ',':
	case '(':
	case ')':
	case ';':
		return 1;
	}
	return 0;
}

// text can have either alphanumeric characters or underscores
// text will either be label or instruction mnemonic
static int is_valid_token_char(const char c)
{
	// isalnum() is case-insensitive, although lex_text()
	// guarantees all-uppercase input
	if (isalnum(c) || c == '_')
		return 1;
	return 0;
}

/* lex_text()
	@tk             ptr to Token struct
	@buffer         ptr aligned to a token in a line of source code
	@instr          ptr to Instruction struct

	@return         number of chars read, or error code

	Lexically analyze a token at @buffer and update @tk to represent that
	token. @instr may also be updated if the token was an instruction
	mnemonic.

	If no valid text is found, @tk is unchanged. If no valid instruction
	mnemonic is found, @instr is unchanged, and @tk will assume a label
	was found.
*/
int lex_text(struct Token *tk, char *buffer, struct Instruction *instr)
{
	char *text = calloc(MAX_TOKEN_STR_LEN, sizeof(char));
	if (!text)
		return ERROR_MEMORY_ALLOCATION_FAIL;

	char c;
	int i;
	for (i = 0; i < MAX_TOKEN_STR_LEN; i++) {
		c = buffer[i];
		if (is_end_of_token(c)) {
			break;
		} else if (!is_valid_token_char(c)) {
			free(text);
			return ERROR_ILLEGAL_CHAR;
		}
		// this assembler is case-insensitive
		// everything defaults to UPPERCASE
		text[i] = toupper(c);
	}

	if (i == MAX_TOKEN_STR_LEN) {
		// the for loop ran its entire course which means there is no
		// space for the null terminator
		free(text);
		return ERROR_TOO_LONG_LABEL;
	}
	text[i] = '\0';
	int num_chars = strlen(text);

	// copy text into token string
	init_token_str(tk, text);
	free(text);

	int lex_instr = lex_instruction(tk, instr);
	if (lex_instr == ERROR_INSTRUCTION_NOT_FOUND) {
		// non-mnemonic text can only be a label
		tk->type = TOKEN_LABEL;
		return num_chars;
	}
	return num_chars;
}