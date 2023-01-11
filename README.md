# Gaia Assembler

Gaia is a single-pass assembler for 6502 assembly. Labels and instructions are
case-insensitive.

# Building

`gcc` and `make` are required to compile the assembler. Run `make` in the root
folder. Usage is `$ ./gaia source_file.asm` which will produce a binary file
`a.out`. Specify an output file name with the option `-o output_file`. Run
`$ ./gaia --help` for more info.

# Current Limitations

- Only supported on Linux/WSL.
- No colons in labels.
- Forward references are only allowed in branch and jump instructions.
- Maximum label length is 63 characters.
- You cannot specify the accumulator as an operand, e.g. `ASL A` is illegal.
You must do `ASL` alone.
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