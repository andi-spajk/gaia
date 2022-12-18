/*
TOKEN SEQUENCES:
InstructionTree
	instruction
		open parenthesis
			operand
				comma
					x_register
						close parenthesis
							null
				close parenthesis
					comma
						y_register
							null
					null
		operand
			comma
				x_register
				y_register
					null
			null
		immediate
			operand
				null
		null

LabelTree
	label
		equal_sign
			literal
				null
		InstructionTree
		null

;----------------------------------------------
; EXHAUSTIVE EXAMPLES OF TOKEN SEQUENCES:
ADDRESS		=	$ABCD
CONSTANT8	=	$7F
CONSTANT16	=	$1111
LABEL1
		INX
		ORA	$AA
		ASL	CONSTANT8
		LDA	$1234
		JMP	LABEL1
		SBC	$BB,X
		ROR	CONSTANT8,X
		INC	$1000,X
		CMP	CONSTANT16,X
		STX	$CC,Y
		LDX	$CONSTANT8,Y
		STA	$DEAD,Y
		AND	ADDRESS,Y
		JMP	($FACE)
		JSR	(ADDRESS)
		EOR	#$DD
		LDY	#CONSTANT8
		ADC	($EE,X)
		LDA	(CONSTANT8,X)
		STA	($FF),Y
		SBC	(CONSTANT8),Y
L1		DEY
L2		ROL	$AA
L3		STY	CONSTANT8
L4		LSR	$1234
L5		JSR	LABEL1
L6		LDY	$BB,X
L7		DEC	CONSTANT8,X
L8		INC	$1000,X
L9		CMP	CONSTANT16,X
L10		STX	$CC,Y
L11		LDX	$CONSTANT8,Y
L12		STA	$DEAD,Y
L13		AND	ADDRESS,Y
L14		JMP	($FACE)
L15		JSR	(ADDRESS)
L16		EOR	#$DD
L17		LDY	#CONSTANT8
L18		ADC	($EE,X)
L19		LDA	(CONSTANT8,X)
L20		STA	($FF),Y
L21		SBC	(CONSTANT8),Y
*/

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "opcode.h"
#include "symbol_table.h"

#define PARSER_SUCCESS           1
#define BRANCH_OPERAND           2
#define JUMP_OPERAND             3
#define BRANCH_FORWARD_REFERENCE 4
#define JUMP_FORWARD_REFERENCE   5

int parse_instr_tree(struct Token **seq, int index);
int parse_label_tree(struct Token **seq, int index);
int parse_line(struct Lexer *lexer);
int parse_label_declaration(struct Lexer *lexer, struct SymbolTable *symtab,
                            int pc);
int parse_label_operand(struct Token *operand, struct Instruction *instr,
                        struct SymbolTable *symtab);
struct Token *get_operand(struct Lexer *lexer);
// int parse_operand(struct Lexer *lexer, struct SymbolTable *symtab,
//                   struct Instruction *instr, int pc);

#endif