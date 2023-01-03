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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "lexer.h"
#include "opcode.h"

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
	tk->str = calloc(MAX_TOKEN_STR_LEN, sizeof(char));
	if (!tk->str)
		return NULL;
	tk->value = 0U;
	tk->error_char = NULL;
	tk->buffer_location = NULL;
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
	lexer->error_tk = NULL;
	lexer->line = NULL;
	return lexer;
}

/* reset_lexer()
	@lexer          ptr to Lexer struct

	Reset the @lexer and all its tokens to null tokens. This prepares the
	lexer for the next source line.
*/
void reset_lexer(struct Lexer *lexer)
{
	for (int i = 0; i < MAX_TOKENS; i++) {
		lexer->sequence[i]->type = TOKEN_NULL;
		lexer->sequence[i]->value = 0;
		lexer->sequence[i]->error_char = NULL;
		lexer->sequence[i]->buffer_location = NULL;
		// no need to wipe the string's contents
		// lexing functions will do that automatically without trouble
	}
	lexer->curr = 0;
	lexer->error_tk = NULL;
	lexer->line = NULL;
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
	if (!lexer)
		return;

	struct Token *curr;
	for (int i = 0; i < MAX_TOKENS; i++) {
		curr = lexer->sequence[i];
		destroy_token(curr);
	}
	free(lexer->sequence);
	free(lexer);
}

/* token_strcpy()
	@tk             ptr to Token struct
	@str            string to duplicate into @tk. Can be empty string

	@return         number of characters in @str, excluding null terminator

	Duplicates a string into @tk's string member. If @str is empty,
	nothing happens.

	This function takes in a string of unknown length and figures out the
	length. This is useful because we keep a single, running token to
	lexically analyze the whole line. At each token, we copy the string of
	the running token into the string of the lexer token. Since we do not
	save the length of the string, this function will figure it out.
*/
int token_strcpy(struct Token *tk, const char *str)
{
	if (!str || *str == '\0')
		return 0;

	// include null terminator to cut off the chars that may be left over
	// from the prior string
	size_t length = strlen(str) + 1;
	strncpy(tk->str, str, length);
	return length - 1;
}

/* token_strncpy()
	@tk             ptr to Token struct
	@buffer         ptr to line of source code
	@length         number of chars to copy, excluding null terminator

	@return         @length

	Copy @length amount of chars from @buffer into the token string.
	All characters will be converted to uppercase. A null terminator will
	be appended.

	This function takes in a known length and copies that many chars from
	@buffer. This is needed during lex_text() because that function calls
	token_strncpy() to copy from buffer to token string (external source
	to internal), but token_strcpy() copies from token string to token
	string (all internal). We must extract only the safe chars from external
	input and convert them to uppercase.
*/
int token_strncpy(struct Token *tk, const char *buffer, int length)
{
	for (int i = 0; i < length; i++) {
		// all UPPERCASE because this assembler is case-insensitive
		tk->str[i] = toupper(buffer[i]);
	}
	tk->str[length] = '\0';
	return length;
}

/* add_token()
	@lexer          ptr to Lexer struct
	@tk             ptr to Token struct with populated members

	@return         success or error code

	Places @tk into the lexer sequence by copying @tk's data into the
	sequence's token. Token is placed in the last free position, if
	able.
*/
int add_token(struct Lexer *lexer, const struct Token *tk)
{
	// end of sequence array
	if (lexer->curr == MAX_TOKENS)
		return ERROR_TOO_MANY_TOKENS;

	struct Token *new = lexer->sequence[lexer->curr];
	if (new->type == TOKEN_NULL) {
		new->type = tk->type;
		token_strcpy(new, tk->str);
		new->value = tk->value;
		// no need to transfer error_char
		// we'll never add tokens that have lexical errors
		new->buffer_location = tk->buffer_location;
		lexer->curr++;
		return TOKEN_INSERTION_SUCCESS;
	}
	return ERROR_UNKNOWN;
}

/* hex_digit_to_int()
	@c              char to check

	@return         integer value of @c, or error code

	Convert a hexadecimal digit to its integer value.
*/
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

/* bin_digit_to_int()
	@c              char to check

	@return         integer value of @c, or error code

	Convert a binary digit to its integer value.
*/
static int bin_digit_to_int(const char c)
{
	if (c == '0')
		return 0;
	else if (c == '1')
		return 1;
	return ERROR_ILLEGAL_CHAR;
}

/* dec_digit_to_int()
	@c              char to check

	@return         integer value of @c, or error code

	Convert a decimal digit to its integer value.
*/
static int dec_digit_to_int(const char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	return ERROR_ILLEGAL_CHAR;
}

/* is_end_of_token()
	@c              char to check

	@return         1 if end of token, 0 if not

	Check if a text token's character denotes the end of the token.
*/
static int is_end_of_token(const char c)
{
	switch (c) {
	case ' ':
	case '\t':
	case '\n':
	case '\0':
	case ',':
	case '(':
	case ')':
	case ';':
	case EOF:
		return 1;
	}
	return 0;
}

/* lex_literal()
	@tk             ptr to Token in the lexer sequence
	@literal        ptr to first char of the literal token

	@return         number of chars read, or error code

	Lexically analyze a literal token and update @tk to hold the literal's
	information.
*/
int lex_literal(struct Token *tk, const char *literal)
{
	// array of ptrs to converter function
	int (*converters[])(char) = {hex_digit_to_int, bin_digit_to_int,
	                             dec_digit_to_int};
	// for indexing array of function ptrs
	int (*converter_func)(char);

	const char *curr = literal;
	int base;
	int num_chars = 0;
	if (*curr == '$') {
		base = 16;
		converter_func = converters[0];
		curr++;
		num_chars++;
	} else if (*curr == '%') {
		base = 2;
		converter_func = converters[1];
		curr++;
		num_chars++;
	} else if (*curr >= '0' && *curr <= '9') {
		base = 10;
		converter_func = converters[2];
	} else {
		tk->error_char = curr;
		return ERROR_ILLEGAL_CHAR;
	}

	// analyze all the digits and convert to actual value
	unsigned long long total = 0;
	int char_value;
	while (*curr) {
		char_value = (*converter_func)(*curr);
		if (char_value != ERROR_ILLEGAL_CHAR) {
			total *= base;
			total += char_value;
			num_chars++;
			curr++;
		} else if (!is_end_of_token(*curr)) {
			// illegal char but not the end of token
			// so it's a char that's incompatible with the base
			tk->error_char = curr;
			return ERROR_ILLEGAL_CHAR;
		} else {
			// illegal and end of token
			break;
		}
	}

	// no valid chars were found
	if (num_chars == 0) {
		tk->error_char = curr;
		return ERROR_ILLEGAL_CHAR;
	} else if (num_chars == 1 && (base == 2 || base == 16)) {
		tk->error_char = curr;
		return ERROR_ILLEGAL_CHAR;
	}

	if (total > 0xFFFF) {
		tk->error_char = literal;
		return ERROR_TOO_BIG_LITERAL;
	}

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

/* is_valid_token_char()
	@c              char to check

	@return         1 if valid character, 0 if not

	Check whether a character is alphanumeric or an underscore since those
	are the only legal characters in labels or instructions.
*/
static int is_valid_token_char(const char c)
{
	// isalnum() is case-insensitive, although lex_text()
	// guarantees all-uppercase input
	return isalnum(c) || c == '_';
}

/* lex_text()
	@buffer         ptr aligned to a token in a line of source code
	@tk             ptr to Token struct
	@instr          ptr to Instruction struct

	@return         number of chars read, or error code

	Lexically analyze a TEXT TOKEN at @buffer, which is a label, register,
	or an instruction mnemonic. Ipdate @tk to represent that token.
	@instr may also be updated if the token was an instruction mnemonic.

	If no valid text is found, @tk is unchanged. If no valid instruction
	mnemonic is found, @instr is unchanged, and @tk will assume a label
	was found.
*/
int lex_text(const char *buffer, struct Token *tk, struct Instruction *instr)
{
	char c;
	int num_chars;
	for (num_chars = 0; num_chars < MAX_TOKEN_STR_LEN; num_chars++) {
		c = buffer[num_chars];
		if (is_end_of_token(c)) {
			break;
		} else if (!is_valid_token_char(c)) {
			tk->error_char = &buffer[num_chars];
			return ERROR_ILLEGAL_CHAR;
		}
	}

	if (num_chars == MAX_TOKEN_STR_LEN) {
		// the for loop ran its entire course which means there is no
		// space for the null terminator
		tk->error_char = buffer;
		return ERROR_TOO_LONG_LABEL;
	}

	// copy the token from buffer to token string
	token_strncpy(tk, buffer, num_chars);

	if (!strcmp(tk->str, "X")) {
		tk->type = TOKEN_X_REGISTER;
		return num_chars;
	} else if (!strcmp(tk->str, "Y")) {
		tk->type = TOKEN_Y_REGISTER;
		return num_chars;
	}

	int lex_instr = lex_instruction(tk, instr);
	if (lex_instr == ERROR_INSTRUCTION_NOT_FOUND)
		tk->type = TOKEN_LABEL;
	// else token is instruction, lex_instruction() already modified *instr
	// and updated tk->type for us
	return num_chars;
}

/* lex()
	@buffer         ptr aligned to a token in a line of source code
	@tk             ptr to Token struct
	@instr          ptr to Instruction struct

	@return         number of chars read, or error code

	Lexically analyze a token at @buffer and update @tk to represent
	that token's meaning. @instr may also be updated if @tk is an
	instruction token.
*/
int lex(const char *buffer, struct Token *tk, struct Instruction *instr)
{
	char c = buffer[0];
	tk->buffer_location = buffer;
	switch (c) {
	case '#':
		tk->type = TOKEN_IMMEDIATE;
		token_strncpy(tk, "#", 1);
		return 1;
	case '(':
		tk->type = TOKEN_OPEN_PARENTHESIS;
		token_strncpy(tk, "(", 1);
		return 1;
	case ')':
		tk->type = TOKEN_CLOSE_PARENTHESIS;
		token_strncpy(tk, ")", 1);
		return 1;
	case ',':
		tk->type = TOKEN_COMMA;
		token_strncpy(tk, ",", 1);
		return 1;
	case '=':
		tk->type = TOKEN_EQUAL_SIGN;
		token_strncpy(tk, "=", 1);
		return 1;
	}

	if (isalpha(c))
		return lex_text(buffer, tk, instr);
	else if (c == '$' || c == '%' || (c >= '0' && c <= '9'))
		return lex_literal(tk, buffer);
	tk->error_char = buffer;
	return ERROR_ILLEGAL_CHAR;
}

/* end_line_lexing()
	@c      current char to check

	Check if the current character in a line of source code is a signal to
	end the lexical analysis. Newlines, comments, null terminators, and EOF
	are all signals that the source code has ended.
*/
static int end_line_lexing(char c)
{
	switch (c) {
	case '\n':
	case ';':
	case '\0':
	case EOF:
		return 1;
	}
	return 0;
}

/* lex_line()
	@buffer         ptr to source line of code
	@lexer          ptr to Lexer struct
	@tk             ptr to running token
	@instr          ptr to Instruction struct

	@return         success or error code

	Lexically analyze an entire line of source code. Place (up to 8) valid
	tokens into @lexer's token sequence and possibly modify @instr to match
	the instruction in the source line. Use @tk as a running token to
	temporarily hold each actual token as we analyze.

	Any information previously inside the lexer or instruction will be
	wiped before proceeding to the lexical analysis.
*/
int lex_line(const char *buffer, struct Lexer *lexer, struct Token *tk,
             struct Instruction *instr)
{
	reset_lexer(lexer);
	reset_instruction(instr);

	// skip whitespace at BEGINNING OF LINE
	const char *curr = buffer;
	while (*curr == ' ' || *curr == '\t')
		curr++;

	// tokenize and perform lexical analysis on every token
	int num_chars;
	while (!end_line_lexing(*curr)) {
		num_chars = lex(curr, tk, instr);
		// negative returns indicate an error code
		if (num_chars < 0) {
			print_error(buffer, num_chars, tk->error_char);
			return num_chars;
		}
		if (add_token(lexer, tk)  == ERROR_TOO_MANY_TOKENS) {
			// don't set lexer->error_tk or call print_error()
			// the parser will pinpoint the first bad token
			return ERROR_TOO_MANY_TOKENS;
		}

		curr += num_chars;
		// skip whitespace AFTER A TOKEN
		while (*curr == ' ' || *curr == '\t')
			curr++;
	}

	// fill the rest of lexer's sequence with null tokens
	int curr_token = lexer->curr;
	while (curr_token < MAX_TOKENS) {
		lexer->sequence[curr_token]->type = TOKEN_NULL;
		curr_token++;
	}
	lexer->line = buffer;
	return LEXER_SUCCESS;
}