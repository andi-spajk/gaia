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
	@ref            ptr to forward reference

	Free the memory used by a dynamically allocated ForwardRef struct.
*/
void destroy_forward_ref(struct ForwardRef *ref)
{
	if (ref->label)
		free(ref->label);
	if (ref->source_line)
		free(ref->source_line);
	if (ref->instr)
		destroy_instruction(ref->instr);
	free(ref);
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

	// all forwardrefs are stored linearly
	for (int i = 0; unresolved->refs[i] != NULL; i++)
		destroy_forward_ref(unresolved->refs[i]);
	free(unresolved->refs);
	free(unresolved);
}

/* create_forward_ref()
	@buffer                 ptr to source line
	@instr                  ptr to Instruction struct
	@operand                ptr to operand token with the forward-referenced
	                        label
	@operand_status         whether operand is branch/jmp and/or a forward
	                        ref, from parse_operand()
	@pc                     current program counter
	@line_num               line number of @buffer

	@return                 ptr to dynamically allocated ForwardRef, or
	                        NULL if fail

	Dynamically allocates a ForwardRef and stores the necessary information
	for resolution and assembling later.

	The saved source line will have NO trailing whitespace.
*/
struct ForwardRef *create_forward_ref(const char *buffer,
                                      const struct Instruction *instr,
                                      const struct Token *operand,
                                      int operand_status, int pc, int line_num)
{
	struct ForwardRef *ref = malloc(sizeof(struct ForwardRef));
	if (!ref)
		return NULL;

	// save source line without trailing whitespace
	// -1 to skip null terminator, -1 again to skip newline
	const char *end = buffer + strlen(buffer) - 2;
	while (*end == ' ' || *end == '\t')
		end--;
	const char *begin = buffer;

	// copy source line into forward ref
	size_t num_chars = end - begin + 1;
	ref->source_line = calloc(num_chars + 1, sizeof(char));
	if (!ref->source_line)
		return NULL;
	// keep the comments in, no one cares
	strncpy(ref->source_line, begin, num_chars);

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

	// DO NOT SET operand_location TO @operand->buffer_location !!!!!
	// we need operand_location and source_line to share memory space so
	// error.c can print the errow arrow correctly
	// source_line is dynamically allocated and is thus different memory
	// from @operand's string
	size_t dist_to_operand = operand->buffer_location - buffer;
	ref->operand_location = ref->source_line + dist_to_operand;

	return ref;
}

/* resize_unresolved()
	@unresolved     ptr to Unresolved struct

	@return         ptr to Unresolved, or NULL if fail

	Dynamically reallocate the memory of an array of forward references in
	an Unresolved struct.
*/
struct Unresolved *resize_unresolved(struct Unresolved *unresolved)
{
	size_t new_size = 2 * unresolved->size * sizeof(struct ForwardRef *);
	unresolved->refs = realloc(unresolved->refs, new_size);
	if (!unresolved->refs)
		return NULL;

	// initialize new forward refs
	unresolved->size *= 2;
	for (int i = unresolved->size / 2; i < unresolved->size; i++)
		unresolved->refs[i] = NULL;

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