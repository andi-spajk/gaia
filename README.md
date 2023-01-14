# Gaia Assembler

Gaia is a single-pass assembler for 6502 assembly. This assembler is
case-insensitive.

# Building

`gcc` and `make` are required to compile the assembler. Run `make` in the root
folder.

Usage is `$ ./gaia source_file.asm` which will produce a binary file `a.out`.

Specify an output file name with the option `-o output_file`.

Run `$ ./gaia --help` for more info.

# Examples

There are example programs in /example/ which were taken and/or adapted from:
- http://www.6502.org/source/
- https://en.wikipedia.org/wiki/MOS_Technology_6502#Example_code

They can be assembled with Gaia. I compared the assembly with the output of
other assemblers like masswerk's and 64tass:
- https://www.masswerk.at/6502/assembler.html
- https://sourceforge.net/projects/tass64/

# Assembler Directives

The following are accepted directives of the Gaia assembler and their syntax.
- `.DEFINE LABEL $ABCD`
- `LABEL .EQU $80`
- `.ORG $1000`
- `.END`
- `*=$1000`
- Values and `*` are hexadecimal in these examples but they can be any numeral
base.

# Current Limitations

- Only supported on Linux/WSL.
- Forward references are only allowed in branch and jump instructions.
- Maximum label length is 63 characters.
- Operands can only be basic literals and labels, e.g. `#"?"` is illegal.
- No arithmetic on operands, e.g. `CPX NUM-1` is illegal.

# Unit Tests

Unit tests are done in /tests/. The Makefiles and the Unity framework are both
used and included; no extra configuration or install is necessary. Source code
at https://github.com/ThrowTheSwitch/Unity.

# Other Credits

- https://www.masswerk.at/6502/6502_instruction_set.html
- https://www.acrc.bris.ac.uk/acrc/RedHat/rhel-cpp-en-4/macro-pitfalls.html

The opcode csv file was taken from:
https://github.com/kpmiller/emulator101/tree/master/6502Disassembler
- NOTE: the original csv source has an error. Opcode for ROR absolute address
and opcode for ROR absolute,X address are swapped.