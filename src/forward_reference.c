/** forward_reference.c

Module for creating and saving forward references in a special separate array
for later processing.

Initialize and destroy forward reference array, create forref objects, and add
them to the array. Automatically resizes array when full.
*/

#include <stdlib.h>
#include <string.h>

#include "forward_reference.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"

#define BASE_SIZE 8

/* init_unresolved()
	@return         ptr to new Unresolved struct, or NULL if fail

	Dynamically allocate a ForwardRef struct and initialize its array of
	ForwardRef elements.
*/
struct Unresolved *init_unresolved(void)
{
	struct Unresolved *unresolved = malloc(sizeof(struct Unresolved));
	if (!unresolved)
		return NULL;

	struct ForwardRef **refs = calloc(BASE_SIZE,
	                                  sizeof(struct ForwardRef *));
	if (!refs)
		return NULL;

	unresolved->refs = refs;
	unresolved->curr = 0;
	unresolved->size = BASE_SIZE;
	return unresolved;
}

/* destroy_forward_ref()
	@forward_ref    ptr to ForwardRef struct

	Free the memory used by a dynamically allocated ForwardRef struct.
*/
void destroy_forward_ref(struct ForwardRef *forward_ref)
{
	if (forward_ref->label)
		free(forward_ref->label);
	if (forward_ref->source_line)
		free(forward_ref->source_line);
	if (forward_ref->instr)
		destroy_instruction(forward_ref->instr);
	free(forward_ref);
}

/* destroy_unresolved()
	@unresolved     ptr to Unresolved struct

	Free the memory used by a dynamically allocated Unresolved struct.
*/
void destroy_unresolved(struct Unresolved *unresolved)
{
	if (!unresolved)
		return;
	if (!unresolved->refs)
		return;

	for (int i = 0; i < unresolved->size; i++) {
		if (unresolved->refs[i])
			destroy_forward_ref(unresolved->refs[i]);
	}
	free(unresolved->refs);
	free(unresolved);
}

/* create_forward_ref()
	@

	@return         ptr to dynamically allocated ForwardRef, or NULL if fail

	Dynamically allocates a ForwardRef and stores the necessary information
	for resolution and assembling later.
*/
struct ForwardRef *create_forward_ref(const char *buffer, struct Lexer *lexer,
                                      struct Instruction *instr, int pc,
                                      int line_num, int operand_status)
{
	struct ForwardRef *ref = malloc(sizeof(struct ForwardRef));
	if (!ref)
		return NULL;

	// save source line
	const char *begin = buffer;
	const char *end = buffer + strlen(buffer) - 1;
	while (*begin == ' ' || *begin == '\t')
		begin++;
	while (*end == ' ' || *end == '\t')
		end--;
	size_t line_length = end - begin + 1;

	ref->source_line = calloc(line_length + 1, sizeof(char));
	if (!ref->source_line)
		return NULL;
	// keep the comments and newline in, no one cares
	strncpy(ref->source_line, begin, line_length);

	// find label operand (from parser.c)
	struct Token *operand = find_operand(lexer);
	// copy label string into forward ref's label
	size_t label_length = strlen(operand->str) + 1;
	ref->label = calloc(label_length, sizeof(char));
	if (!ref->label)
		return NULL;
	strncpy(ref->label, operand->str, label_length);

	// copy instr into forward ref's instr
	struct Instruction *new_instr = init_instruction();
	if (!new_instr)
		return NULL;
	new_instr->mnemonic = instr->mnemonic;
	new_instr->addr_bitfield = instr->addr_bitfield;
	new_instr->addr_bitflag = instr->addr_bitflag;
	new_instr->opcode = instr->opcode;
	ref->instr = new_instr;

	ref->pc = pc;
	ref->line_num = line_num;
	ref->operand_status = operand_status;

	return ref;
}