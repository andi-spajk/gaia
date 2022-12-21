/** forward_reference.c

Module for creating and saving forward references in a special separate array
for later processing.

Initialize and destroy forward reference array, create forref objects, and add
them to the array. Automatically resizes array when full.
*/

#include <stdlib.h>

#include "forward_reference.h"
#include "opcode.h"

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
static void destroy_forward_ref(struct ForwardRef *forward_ref)
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