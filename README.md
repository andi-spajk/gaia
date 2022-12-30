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