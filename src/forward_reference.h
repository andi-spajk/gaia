/** forward_reference.h

Prototypes and structs that init, save, resolve, and destroy forward references
in a single Unresolved struct.

*/

#ifndef FORWARD_REFERENCE_H
#define FORWARD_REFERENCE_H

#include "lexer.h"
#include "opcode.h"

#define FORWARD_REFERENCE_INSERTION_SUCCESS 1

struct ForwardRef {
	char *label;                   // forward-referenced label
	char *source_line;             // the full source line of code
	struct Instruction *instr;     // the instruction of the line
	int pc;                        // location of instruction's opcode in the
	                               // assembled binary file
	int line_num;
	int operand_status;            // whether instr is branch or jump
	const char *operand_location;  // location of operand in source_line
};

struct Unresolved {
	struct ForwardRef **refs;  // linear array of ForwardRef
	int curr;                  // first open index in refs[]
	int size;
};

struct Unresolved *init_unresolved(void);
void destroy_forward_ref(struct ForwardRef *forward_ref);
void destroy_unresolved(struct Unresolved *unresolved);
struct ForwardRef *create_forward_ref(const char *buffer,
                                      const struct Instruction *instr,
                                      const struct Token *operand,
                                      int operand_status, int pc, int line_num);
int add_forward_ref(struct Unresolved *unresolved, struct ForwardRef *ref);

#endif