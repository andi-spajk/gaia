# Gaia Assembler

Gaia is a single-pass assembler for 6502 assembly.

# Current Limitations

- Only supported on Linux or WSL.
- No colons in labels.
- Forward references are only allowed in branch and jump instructions.
- Maximum label length is 63 characters.
- No assembler directives.
- You cannot specify the accumulator as an operand, e.g. `ASL A` is illegal.
You must do `ASL` alone.
- No arithmetic on operands, e.g. `CPX NUM-1` is illegal.

# Unit Tests

Unit tests are done in /tests/. The Unity framework is used and included; no
extra configuration or install is necessary. Source code at
https://github.com/ThrowTheSwitch/Unity.

# Other Credits

https://www.masswerk.at/6502/6502_instruction_set.html

The opcode csv file was taken from:
https://github.com/kpmiller/emulator101/tree/master/6502Disassembler