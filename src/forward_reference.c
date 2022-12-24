/** forward_reference.c

Module for creating and saving forward references in a special separate array
for later processing.

Initialize and destroy forward reference array, create forref objects, and add
them to the array. Automatically resizes array when full.
*/

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "forward_reference.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"

#define BASE_SIZE 8

/* init_unresolved()
	@return         ptr to new Unresolved struct, or NULL if fail

	Dynamically allocate an Unresolved struct and initialize its array of
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
	@buffer                 ptr to source line
	@lexer                  ptr to Lexer struct
	@instr                  ptr to Instruction struct
	@pc                     current program counter
	@line_num               line number of @buffer
	@operand_status         whether operand is branch/jmp and/or a forward
	                        ref, from parse_operand()

	@return                 ptr to dynamically allocated ForwardRef, or
	                        NULL if fail

	Dynamically allocates a ForwardRef and stores the necessary information
	for resolution and assembling later.

	The saved source line will have NO trailing whitespace.
*/
struct ForwardRef *create_forward_ref(const char *buffer, struct Lexer *lexer,
                                      struct Instruction *instr, int pc,
                                      int line_num, int operand_status)
{
	struct ForwardRef *ref = malloc(sizeof(struct ForwardRef));
	if (!ref)
		return NULL;

	const char *begin = buffer;
	// save source line without trailing whitespace
	// -1 to skip null terminator, -1 again to skip newline
	const char *end = buffer + strlen(buffer) - 2;
	while (*end == ' ' || *end == '\t')
		end--;
	size_t line_length = end - begin + 1;

	ref->source_line = calloc(line_length + 1, sizeof(char));
	if (!ref->source_line)
		return NULL;
	// keep the comments in, no one cares
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

/* resize_unresolved()
	@unresolved     ptr to Unresolved struct

	@return         ptr to resized struct, or NULL if fail

	Dynamically reallocate the memory of an array of forward references in
	an Unresolved struct.
*/
struct Unresolved *resize_unresolved(struct Unresolved *unresolved)
{
	unresolved->size *= 2;
	// calloc'ing a whole new array and transfering the old elements is
	// easier than realloc'ing because I broke myself trying to initialize
	// the new memory to NULL ptrs
	struct ForwardRef **new = calloc(unresolved->size,
	                                 sizeof(struct ForwardRef *));
	// transfer old forward refs
	for (int i = 0; i < unresolved->size / 2; i++) {
		if (unresolved->refs[i])
			new[i] = unresolved->refs[i];
	}
	// initialize new refs
	for (int j = unresolved->size / 2; j < unresolved->size; j++)
		new[j] = NULL;

	free(unresolved->refs);
	unresolved->refs = new;
	return unresolved;
}

/* add_forward_ref()
	@unresolved     ptr to forward references
	@ref            ptr to new forward reference to add

	@return         success or error code

	Add a forward reference to the array in @unresolved. Resize the array if
	necessary.
*/
int add_forward_ref(struct Unresolved *unresolved, struct ForwardRef *ref)
{
	// array is full
	if (unresolved->curr == unresolved->size - 1) {
		if (!resize_unresolved(unresolved))
			return ERROR_MEMORY_ALLOCATION_FAIL;
	}
	unresolved->refs[unresolved->curr] = ref;
	unresolved->curr++;
	return FORWARD_REFERENCE_INSERTION_SUCCESS;
}