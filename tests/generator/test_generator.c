// The line numbers in the error messages are wrong because I increment line_num
// AFTER lex_line() and then I call create_forward_ref().
// In the actual assembler loop, you wouldn't increment until the very end.
// I am too lazy to fix it here because adding the proper line number was
// already tested in test_forward_reference.c. None of the tests here are broken
// by having wrong line numbers.

#include <stdio.h>
#include <stdlib.h>

#include "../../unity/unity.h"
#include "addressing_modes.h"
#include "bitfields.h"
#include "error.h"
#include "forward_reference.h"
#include "generator.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

void setUp(void) {}
void tearDown(void) {}

void test_generate_code(void)
{
	struct Lexer *lexer = init_lexer("test_generate_code.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = NULL;
	// these tests don't use any labels
	int line_num = 1;

	FILE *f = fopen("generate_code.out", "w+b");
	TEST_ASSERT_NOT_NULL(f);

	/*
	from /tests/parser/exhaustive_lines.h
	E8              INX
	05 AA           ORA     $AA
	AD 34 12        LDA     $1234
	F5 BB           SBC     $BB,X
	FE 00 10        INC     $1000,X
	96 CC           STX     $CC,Y
	99 AD DE        STA     $DEAD,Y
	6C CE FA        JMP     ($FACE)
	49 DD           EOR     #$DD
	61 EE           ADC     ($EE,X)
	D1 FF           CMP     ($FF),Y
	0A              ASL
	*/
	// no illegal instructions because the lexer/parser and caller would
	// snuff them out before generating code
	unsigned char expected_rom[] = {
		0xE8,
		0x05, 0xAA,
		0xAD, 0x34, 0x12,
		0xF5, 0xBB,
		0xFE, 0x00, 0x10,
		0x96, 0xCC,
		0x99, 0xAD, 0xDE,
		0x6C, 0xCE, 0xFA,
		0x49, 0xDD,
		0x61, 0xEE,
		0xD1, 0xFF,
		0x0A
	};
	int exp_bytes = 23;
	unsigned char produced_rom[exp_bytes];

	int pc = 0;
	int written = 0;
	struct Token *operand;
	int operand_status;
	const char *buffer;

	buffer = "INX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "ORA $AA\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc+= written;

	buffer = "LDA $1234\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "SBC $BB,X\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "INC $1000,X\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "STX $CC, Y\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "STA $DEAD,Y\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "JMP ($FACE)\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "EOR #$DD\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "ADC ($EE,X)\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "CMP ($FF),Y\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "ASL\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	fseek(f, 0, SEEK_SET);
	TEST_ASSERT_EQUAL_INT(exp_bytes, fread(produced_rom, sizeof(unsigned char), exp_bytes, f));
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_rom, produced_rom, exp_bytes);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	fclose(f);
}

void test_resolve_label_ref(void)
{
	struct Lexer *lexer = init_lexer("test_resolve_label_ref.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	int line_num = 1;

	FILE *f = fopen("resolve_label_ref.out", "w+b");
	TEST_ASSERT_NOT_NULL(f);

	/*
	LOC     CODE
	                        ADDRESS =       $1234
	1000    E8              LABEL1  INX
	1001    D0 FD                   BNE     LABEL1          ; branch
	1003    4C 00 10                JMP     LABEL1          ; jump
	1006    F0 FE           SAME1   BEQ     SAME1           ; branch to same instr
	1008    4C 08 10        SAME2   JMP     SAME2           ; jump to same instr
	100B    10 F9           LABEL2  BPL     SAME1           ; label branch
	100D    20 0B 10                JSR     LABEL2          ; subroutine jump
	1010    6C 34 12                JMP     (ADDRESS)       ; indirect jump
	*/
	// no illegal instructions because the lexer/parser and caller would
	// snuff them out before generating code
	unsigned char expected_rom[] = {
		0xE8,
		0xD0, 0xFD,
		0x4C, 0x00, 0x10,
		0xF0, 0xFE,
		0x4C, 0x08, 0x10,
		0x10, 0xF9,
		0x20, 0x0B, 0x10,
		0x6C, 0x34, 0x12
	};
	int exp_bytes = 19;
	unsigned char produced_rom[exp_bytes];

	int pc = 0x1000;
	int written = 0;
	const char *buffer;
	struct Token *operand;
	int operand_status;

	// ADDRESS = $1234
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, insert_symbol(symtab, "ADDRESS", 0x1234));
	TEST_ASSERT_EQUAL_INT(0x1234, search_symbol(symtab, "ADDRESS"));
	line_num++;

	buffer = "LABEL1 INX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "BNE LABEL1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	TEST_ASSERT_EQUAL_INT(0x1000, operand->value);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(0xFD, operand->value);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "JMP LABEL1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "SAME1 BEQ SAME1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "SAME2 JMP SAME2\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "LABEL2 BPL SAME1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "JSR LABEL2\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "JMP (ADDRESS)\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	fseek(f, 0x1000, SEEK_SET);
	TEST_ASSERT_EQUAL_INT(exp_bytes, fread(produced_rom, sizeof(unsigned char), exp_bytes, f));
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_rom, produced_rom, exp_bytes);

	insert_symbol(symtab, "FARAWAY", 0xF000);
	buffer = "\t\tBMI\tFARAWAY\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	fclose(f);
}

void test_too_big_offset_no_forward_ref(void)
{
	struct Lexer *lexer = init_lexer("test_too_big_offset_no_forward_ref.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

	FILE *f = fopen("too_big_offset_no_forward_ref.out", "w+b");
	TEST_ASSERT_NOT_NULL(f);

	/*
	max offset
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  E8           BACK      INX
	007E                         * = $007E
	007E  90 80                  BCC BACK
	*/
	const char *buffer;
	struct Token *operand;
	int operand_status;
	int written;
	int pc;
	int line_num = 1;

	pc = 0x0;
	buffer = "BACK INX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);

	pc = 0x7E;
	buffer = "BCC BACK\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(2, written);

	/*
	edge case
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  E8           BACK      INX
	007F                         * = $007F
	007F                         BCC BACK
	*/
	line_num = 2;
	pc = 0x7F;
	buffer = "BCC\tBACK\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	/*
	definitely bad offset
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  E8           BACK      INX
	0ABC                         * = $0ABC
	0ABC                         BCC BACK
	*/
	line_num = 2;
	pc = 0xABC;
	buffer = "                BCC     BACK\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = resolve_label_ref(f, lexer, instr, operand, operand_status, symtab, pc);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	fclose(f);
}

void test_resolve_forward_ref(void)
{
	struct Lexer *lexer = init_lexer("test_resolve_forward_ref.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);
	int line_num = 1;

	FILE *f = fopen("resolve_forward_ref.out", "w+b");
	TEST_ASSERT_NOT_NULL(f);

	/*
	LOC   CODE         LABEL     INSTRUCTION
	0000  70 09                  BVS L1
	0002  4C 0C 00               JMP L2
	0005  20 0D 00               JSR L3
	0008  6C CD AB               JMP (WHERE)
	000B  E8           L1        INX
	000C  C8           L2        INY
	000D  CA           L3        DEX
	                             WHERE = $ABCD
	*/
	// no illegal instructions because the lexer/parser and caller would
	// snuff them out before resolving forward refs
	unsigned char expected_rom[] = {
		0x70, 0x09,
		0x4C, 0x0C, 0x00,
		0x20, 0x0D, 0x00,
		0x6C, 0xCD, 0xAB,
		0xE8,
		0xC8,
		0xCA
	};
	int exp_bytes = 14;
	unsigned char produced_rom[exp_bytes];

	const char *buffer;
	struct Token *operand;
	int operand_status;
	int pc = 0x0;
	struct ForwardRef *ref;
	int written;

	buffer = "BVS L1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	// dummy bytes
	fputc(0x00, f);
	fputc(0x00, f);
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	pc += 2;

	buffer = "JMP L2\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	pc += 3;

	buffer = "JSR L3\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	pc += 3;

	buffer = "JMP (WHERE)\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));
	fputc(0x00, f);
	fputc(0x00, f);
	fputc(0x00, f);
	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	TEST_ASSERT_EQUAL_INT(4, unresolved->curr);
	pc += 3;

	buffer = "L1 INX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "L2 INY\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "L3 DEX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "WHERE = $ABCD\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	TEST_ASSERT_EQUAL_INT(NULL_MNEMONIC, instr->mnemonic);

	int dest_pc;
	ref = unresolved->refs[0];
	for (int i = 0; i < unresolved->curr; i++) {
		ref = unresolved->refs[i];
		dest_pc = search_symbol(symtab, ref->label);
		resolve_forward_ref(f, ref, lexer, dest_pc);
	}

	fseek(f, 0, SEEK_SET);
	TEST_ASSERT_EQUAL_INT(exp_bytes, fread(produced_rom, sizeof(unsigned char), exp_bytes, f));
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_rom, produced_rom, exp_bytes);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	destroy_unresolved(unresolved);
	fclose(f);
}

void test_too_big_offset_with_forward_ref(void)
{
	struct Lexer *lexer = init_lexer("test_too_big_offset_with_forward_ref.asm");
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);
	struct Unresolved *unresolved = init_unresolved();
	TEST_ASSERT_NOT_NULL(unresolved);

	FILE *f = fopen("too_big_offset_with_forward_ref.out", "w+b");
	TEST_ASSERT_NOT_NULL(f);

	/*
	max offset
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  50 7F                  BVC FARAWAY1
	0081                         * = $0081
	0081  C8           FARAWAY1  INY
	*/
	const char *buffer;
	struct Token *operand;
	int operand_status;
	int pc;
	struct ForwardRef *ref;
	int written;
	int line_num = 1;

	pc = 0x0;
	buffer = "BVC FARAWAY1\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	fputc(0x00, f);
	fputc(0x00, f);

	pc = 0x81;
	buffer = "FARAWAY1 INY\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);

	int curr_forward_ref = 0;
	int dest_pc = search_symbol(symtab, unresolved->refs[curr_forward_ref]->label);
	TEST_ASSERT_EQUAL_INT(2, resolve_forward_ref(f, unresolved->refs[curr_forward_ref], lexer, dest_pc));
	curr_forward_ref++;

	/*
	edge case
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000                         BCS FARAWAY2
	0082                         * = $0082
	0082               FARAWAY2  INY
	*/
	line_num = 1;

	pc = 0x0;
	buffer = "\t\tBCS\tFARAWAY2\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	fputc(0x00, f);
	fputc(0x00, f);

	pc = 0x82;
	buffer = "FARAWAY2 INY\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);

	dest_pc = search_symbol(symtab, unresolved->refs[curr_forward_ref]->label);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, resolve_forward_ref(f, unresolved->refs[curr_forward_ref], lexer, dest_pc));
	curr_forward_ref++;

	/*
	definitely bad offset
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000                         BVS FARAWAY3
	1000                         * = $1000
	1000               FARAWAY3  DEX
	*/
	line_num = 1;

	pc = 0x0;
	buffer = "BVS FARAWAY3\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE, parse_addr_mode(lexer, instr, operand, operand_status));

	ref = create_forward_ref(buffer, instr, operand, operand_status, pc, line_num);
	TEST_ASSERT_NOT_NULL(ref);
	TEST_ASSERT_EQUAL_INT(FORWARD_REFERENCE_INSERTION_SUCCESS, add_forward_ref(unresolved, ref));
	fputc(0x00, f);
	fputc(0x00, f);

	pc = 0x1000;
	buffer = "FARAWAY3 DEX\n";
	lex_line(buffer, lexer, tk, instr, line_num++);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(lexer, instr, operand, symtab);
	parse_addr_mode(lexer, instr, operand, operand_status);
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);

	dest_pc = search_symbol(symtab, unresolved->refs[curr_forward_ref]->label);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, resolve_forward_ref(f, unresolved->refs[curr_forward_ref], lexer, dest_pc));
	curr_forward_ref++;

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	destroy_unresolved(unresolved);
	fclose(f);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_generate_code);
	RUN_TEST(test_resolve_label_ref);
	RUN_TEST(test_too_big_offset_no_forward_ref);
	RUN_TEST(test_resolve_forward_ref);
	RUN_TEST(test_too_big_offset_with_forward_ref);

	return UNITY_END();
}