# Gaia Assembler

Gaia is a single-pass assembler for 6502 assembly.

# Building

`gcc` and `make` are required to compile the assembler. Run `make` in the root
folder. Usage is `$ ./gaia source_file.asm` which will produce a binary file
`a.out`.

# Current Limitations

- Can't specify a name for the output file, e.g. `./gaia source.asm -o source`
is not possible.
- No colons in labels.
- Forward references are only allowed in branch and jump instructions.
- Maximum label length is 63 characters.
- No assembler directives.
- You cannot specify the accumulator as an operand, e.g. `ASL A` is illegal.
You must do `ASL` alone.
- Operands can only be basic literals and labels, e.g. `#"?"` is illegal.
- No arithmetic on operands, e.g. `CPX NUM-1` is illegal.
- Unit tests only work on Linux/WSL.

# Unit Tests

Unit tests are done in /tests/. The Makefiles and the Unity framework are both
used and included; no extra configuration or install is necessary. Source code
at https://github.com/ThrowTheSwitch/Unity.

# Other Credits

- https://www.masswerk.at/6502/6502_instruction_set.html
- https://www.acrc.bris.ac.uk/acrc/RedHat/rhel-cpp-en-4/macro-pitfalls.html

The opcode csv file was taken from:
https://github.com/kpmiller/emulator101/tree/master/6502Disassembler