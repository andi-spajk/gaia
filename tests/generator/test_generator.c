#include <stdio.h>
#include <stdlib.h>

#include "../../unity/unity.h"
#include "addressing_modes.h"
#include "bitfields.h"
#include "generator.h"
#include "lexer.h"
#include "opcode.h"

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
	struct Token *operand = init_token();
	TEST_ASSERT_NOT_NULL(operand);
	struct Instruction *instr = init_instruction();
	TEST_ASSERT_NOT_NULL(instr);

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
		0xE8, 0x05, 0xAA, 0xAD, 0x34,
		0x12, 0xF5, 0xBB, 0xFE, 0x00,
		0x10, 0x96, 0xCC, 0x99, 0xAD,
		0xDE, 0x6C, 0xCE, 0xFA, 0x49,
		0xDD, 0x61, 0xEE, 0xD1, 0xFF,
		0x0A
	};
	int exp_bytes = 23;
	unsigned char produced_rom[exp_bytes];

	int pc = 0;
	int written = 0;

	// INX
	operand->value = 0xabcd;
	instr->mnemonic = INX;
	instr->addr_bitfield = INX_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_IMPLIED;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	// ORA $AA
	operand->value = 0xAA;
	instr->mnemonic = ORA;
	instr->addr_bitfield = ORA_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ZERO_PAGE;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc+= written;

	// LDA $1234
	operand->value = 0x1234;
	instr->mnemonic = LDA;
	instr->addr_bitfield = LDA_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ABSOLUTE;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	// SBC $BB,X
	operand->value = 0xBB;
	instr->mnemonic = SBC;
	instr->addr_bitfield = SBC_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ZERO_PAGE_X;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	// INC $1000,X
	operand->value = 0x1000;
	instr->mnemonic = INC;
	instr->addr_bitfield = INC_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ABSOLUTE_X;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	// STX $CC, Y
	operand->value = 0xCC;
	instr->mnemonic = STX;
	instr->addr_bitfield = STX_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ZERO_PAGE_Y;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	// STA $DEAD,Y
	operand->value = 0xDEAD;
	instr->mnemonic = STA;
	instr->addr_bitfield = STA_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ABSOLUTE_Y;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	// JMP ($FACE)
	operand->value = 0xFACE;
	instr->mnemonic = JMP;
	instr->addr_bitfield = JMP_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_INDIRECT;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(3, written);
	pc += written;

	// EOR #$DD
	operand->value = 0xDD;
	instr->mnemonic = EOR;
	instr->addr_bitfield = EOR_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_IMMEDIATE;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	// ADC ($EE,X)
	operand->value = 0xEE;
	instr->mnemonic = ADC;
	instr->addr_bitfield = ADC_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_X_INDEXED_INDIRECT;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	// CMP ($FF),Y
	operand->value = 0xFF;
	instr->mnemonic = CMP;
	instr->addr_bitfield = CMP_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_INDIRECT_Y_INDEXED;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(2, written);
	pc += written;

	// ASL
	operand->value = 0x1234;
	instr->mnemonic = ASL;
	instr->addr_bitfield = ASL_BITFIELD;
	instr->addr_bitflag = ADDR_MODE_ACCUMULATOR;
	written = generate_code(f, instr, operand, pc);
	TEST_ASSERT_EQUAL_INT(1, written);
	pc += written;

	fseek(f, 0, SEEK_SET);
	TEST_ASSERT_EQUAL_INT(exp_bytes, fread(produced_rom, sizeof(unsigned char), exp_bytes, f));
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_rom, produced_rom, exp_bytes);

	destroy_token(operand);
	destroy_instruction(instr);
	fclose(f);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_calc_num_bytes);
	RUN_TEST(test_generate_code);

	return UNITY_END();
}