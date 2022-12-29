#ifndef FORWARD_REFERENCE_H
#define FORWARD_REFERENCE_H

#include "lexer.h"
#include "opcode.h"

#define FORWARD_REFERENCE_INSERTION_SUCCESS 1

struct ForwardRef {
	char *label;
	char *source_line;
	struct Instruction *instr;
	int pc;
	int line_num;
	int operand_status;
};

struct Unresolved {
	struct ForwardRef **refs;
	int curr;  // first open slot in refs[]
	int size;
};

struct Unresolved *init_unresolved(void);
void destroy_forward_ref(struct ForwardRef *forward_ref);
void destroy_unresolved(struct Unresolved *unresolved);
struct ForwardRef *create_forward_ref(const char *buffer, struct Lexer *lexer,
                                      struct Instruction *instr,
                                      int operand_status, int pc, int line_num);
int add_forward_ref(struct Unresolved *unresolved, struct ForwardRef *ref);

#endif