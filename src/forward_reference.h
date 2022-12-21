#ifndef FORWARD_REFERENCE_H
#define FORWARD_REFERENCE_H

#include "opcode.h"

struct ForwardRef {
	char *label;
	char *source_line;
	int line_num;
	int pc;
	struct Instruction *instr;
	int operand_status;
};

struct Unresolved {
	struct ForwardRef **refs;
	int curr;
	int size;
};

struct Unresolved *init_unresolved(void);
void destroy_unresolved(struct Unresolved *forward_refs);

#endif