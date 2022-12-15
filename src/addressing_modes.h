/*
Each bit represents a particular addressing mode
If we figure out whether the operand is absolute or zero page,
we can just mask out the modes with registers and/or indirect parentheses.
For example, an operand with an x register will have a specific bitfield.
However, if the instr is a jump instr, that bitfield will be incompatible
with the previous bitfield, ie they will share no bits.

We start our addressing mode as a field, denoting all possible absolute
modes or zero page modes
Then we mask the x register and y register modes depending on which register
is in the source code
If there is none, we mask the inverse of the registers' modes
Then we mask the indirect modes
If there is none, we mask the inverse of the indirect modes

********* BITFIELDS FORMATTED AND ORGANIZED FOR CLARITY *********
no fields necessary, easy to check
        0b1 0000 0000 0000      ADDR_MODE_ACCUMULATOR
        0b0 1000 0000 0000      ADDR_MODE_IMPLIED

these addressing modes have a zero page byte
it is part of ZERO_PAGE_FIELD because it makes the
semantic checking of the byte size into a simple logical AND
        0b0 0100 0000 0000      ADDR_MODE_IMMEDIATE
        0b0 0010 0000 0000      ADDR_MODE_RELATIVE

there are 4 modes with absolute operands
        0b0 0001 1110 0000      ABSOLUTE_FIELD
        0b0 0001 0000 0000      ADDR_MODE_ABSOLUTE
        0b0 0000 1000 0000      ADDR_MODE_ABSOLUTE_X
        0b0 0000 0100 0000      ADDR_MODE_ABSOLUTE_Y
this addressing mode belongs to two fields <3
        0b0 0000 0010 0000      ADDR_MODE_ABSOLUTE_INDIRECT

there are 3 modes with indirect operands
        0b0 0000 0011 1000      INDIRECT_FIELD
these are all zero page
        0b0 0110 0001 1111      ZERO_PAGE_FIELD
        0b0 0000 0001 0000      ADDR_MODE_X_INDEX_INDIRECT
        0b0 0000 0000 1000      ADDR_MODE_INDIRECT_Y_INDEX
        0b0 0000 0000 0100      ADDR_MODE_ZERO_PAGE
        0b0 0000 0000 0010      ADDR_MODE_ZERO_PAGE_X
        0b0 0000 0000 0001      ADDR_MODE_ZERO_PAGE_Y

                                (ADDR_MODE_ABSOLUTE_X | ADDR_MODE_ZERO_PAGE_X | ADDR_MODE_X_INDEX_INDIRECT)
        0b0 0000 1001 0010      X_REGISTER_FIELD

                                (ADDR_MODE_ABSOLUTE_Y | ADDR_MODE_ZERO_PAGE_Y | ADDR_MODE_INDIRECT_Y_INDEX)
        0b0 0000 0100 1001      Y_REGISTER_FIELD

                                (~(X_REGISTER_FIELD | Y_REGISTER_FIELD))
        0b1 1111 0010 0100      NOT_REGISTER_FIELD

                                (~INDIRECT_FIELD)
        0b1 1111 1100 0111      NOT_INDIRECT_FIELD
*/

#ifndef ADDRESSING_MODES_H
#define ADDRESSING_MODES_H

#define ADDR_MODE_ACCUMULATOR           0b1000000000000
#define ADDR_MODE_IMPLIED               0b0100000000000
#define ADDR_MODE_IMMEDIATE             0b0010000000000
#define ADDR_MODE_RELATIVE              0b0001000000000
#define ADDR_MODE_ABSOLUTE              0b0000100000000
#define ADDR_MODE_ABSOLUTE_X            0b0000010000000
#define ADDR_MODE_ABSOLUTE_Y            0b0000001000000
#define ADDR_MODE_ABSOLUTE_INDIRECT     0b0000000100000
#define ADDR_MODE_X_INDEX_INDIRECT      0b0000000010000
#define ADDR_MODE_INDIRECT_Y_INDEX      0b0000000001000
#define ADDR_MODE_ZERO_PAGE             0b0000000000100
#define ADDR_MODE_ZERO_PAGE_X           0b0000000000010
#define ADDR_MODE_ZERO_PAGE_Y           0b0000000000001

#define ABSOLUTE_FIELD                  0b0000111100000
#define INDIRECT_FIELD                  0b0000000111000
#define ZERO_PAGE_FIELD                 0b0011000011111

#define X_REGISTER_FIELD                0b0000010010010
#define Y_REGISTER_FIELD                0b0000001001001
#define NOT_REGISTER_FIELD              0b1111100100100
#define NOT_INDIRECT_FIELD              0b1111111000111

#endif