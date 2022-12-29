#include <stdio.h>
#include <stdlib.h>

#include "../../unity/unity.h"
#include "addressing_modes.h"
#include "bitfields.h"
#include "error.h"
#include "generator.h"
#include "lexer.h"
#include "opcode.h"
#include "parser.h"
#include "symbol_table.h"

void setUp(void) {}
void tearDown(void) {}

void test_calc_num_bytes(void)
{
	// lexical analyzer screens out any operands greater than 3 bytes
	// so there is no need to check for those
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0x0));
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0x01));
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0x0000000000000001));
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0xFF));
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0x7F));
	TEST_ASSERT_EQUAL_INT(1, calc_num_bytes(0x80));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0x100));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0x0234));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0xAAAA));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0x00001000));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0x8000));
	TEST_ASSERT_EQUAL_INT(2, calc_num_bytes(0xFF00));
}

void test_generate_code(void)
{
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = NULL;
	// these tests don't use any labels

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
	int addr_mask;
	int operand_status;
	const char *buffer;

	buffer = "INX\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "ORA $AA\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc+= written;

	buffer = "LDA $1234\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "SBC $BB,X\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "INC $1000,X\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "STX $CC, Y\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "STA $DEAD,Y\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "JMP ($FACE)\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "EOR #$DD\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "ADC ($EE,X)\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "CMP ($FF),Y\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "ASL\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
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
	struct Lexer *lexer = init_lexer();
	TEST_ASSERT_NOT_NULL(lexer);
	struct Token *tk = init_token();
	TEST_ASSERT_NOT_NULL(tk);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);
	struct SymbolTable *symtab = init_symbol_table();
	TEST_ASSERT_NOT_NULL(symtab);

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
	100D    20 0B 10                JSR     LABEL2          ; label jump
	1010    6C 34 12                JMP     (ADDRESS)       ; indirect jump
	*/
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
	int addr_mask;

	// ADDRESS = $1234
	TEST_ASSERT_EQUAL_INT(SYMBOL_INSERTION_SUCCESS, insert_symbol(symtab, "ADDRESS", 0x1234));
	TEST_ASSERT_EQUAL_INT(0x1234, search_symbol(symtab, "ADDRESS"));

	buffer = "LABEL1 INX\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	buffer = "BNE LABEL1\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	TEST_ASSERT_EQUAL_INT(0x1000, operand->value);
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(0xFD, operand->value);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "JMP LABEL1\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "SAME1 BEQ SAME1\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "SAME2 JMP SAME2\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "LABEL2 BPL SAME1\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	buffer = "JSR LABEL2\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	buffer = "JMP (ADDRESS)\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	fseek(f, 0x1000, SEEK_SET);
	TEST_ASSERT_EQUAL_INT(exp_bytes, fread(produced_rom, sizeof(unsigned char), exp_bytes, f));
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_rom, produced_rom, exp_bytes);

	insert_symbol(symtab, "FARAWAY", 0xF000);
	buffer = "BMI FARAWAY\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	fclose(f);
}

void test_too_big_offset_no_forward_ref(void)
{
	struct Lexer *lexer = init_lexer();
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
	int addr_mask;
	int written;
	int pc;

	pc = 0x0;
	buffer = "BACK INX\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	parse_label_declaration(lexer, symtab, pc);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);

	pc = 0x7E;
	buffer = "BCC BACK\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(2, written);

	/*
	edge case
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  E8           BACK      INX
	007F                         * = $007F
	007F                         BCC BACK
	*/
	pc = 0x7F;
	buffer = "BCC BACK\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	/*
	definitely bad offset
	LOC   CODE         LABEL     INSTRUCTION
	0000                         * = $0000
	0000  E8           BACK      INX
	0ABC                         * = $0ABC
	0ABC                         BCC BACK
	*/
	pc = 0xABC;
	buffer = "BCC BACK\n";
	lex_line(buffer, lexer, tk, instr);
	parse_line(lexer);
	operand = find_operand(lexer);
	operand_status = parse_operand(operand, instr, symtab);
	addr_mask = parse_addr_mode(operand_status, operand, lexer, instr);
	instr->addr_bitflag = instr->addr_bitfield & addr_mask;
	written = resolve_label_ref(f, instr, operand, symtab, pc, operand_status);
	TEST_ASSERT_EQUAL_INT(ERROR_TOO_BIG_OFFSET, written);

	destroy_lexer(lexer);
	destroy_token(tk);
	destroy_instruction(instr);
	destroy_symbol_table(symtab);
	fclose(f);
}

void test_too_big_offset_with_forward_ref(void)
{
	/*
	max offset
	LOC   CODE         LABEL     INSTRUCTION
	0100                         * = $0100
	0100  70 7F                  BVS FARAWAY
	0181                         * = $0181
	0181  C8           FARAWAY   INY
	*/

	/*
	edge case
	LOC   CODE         LABEL     INSTRUCTION
	0100                         * = $0100
	0100                         BVS FARAWAY
	0182                         * = $0182
	0182               FARAWAY   INY
	*/

	/*
	definitely bad offset
	LOC   CODE         LABEL     INSTRUCTION
	1000                         * = $1000
	1000                         BVS FARAWAY
	2000                         * = $2000
	2000               FARAWAY   DEX
	*/
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_calc_num_bytes);
	RUN_TEST(test_generate_code);
	RUN_TEST(test_resolve_label_ref);
	RUN_TEST(test_too_big_offset_no_forward_ref);
	// RUN_TEST(test_too_big_offset_with_forward_ref);

	return UNITY_END();
}