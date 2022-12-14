import csv

ADDR_MODE_ACCUMULATOR           = 0b1000000000000
ADDR_MODE_IMPLIED               = 0b0100000000000
ADDR_MODE_IMMEDIATE             = 0b0010000000000
ADDR_MODE_RELATIVE              = 0b0001000000000
ADDR_MODE_ABSOLUTE              = 0b0000100000000
ADDR_MODE_ABSOLUTE_X            = 0b0000010000000
ADDR_MODE_ABSOLUTE_Y            = 0b0000001000000
ADDR_MODE_ABSOLUTE_INDIRECT     = 0b0000000100000
ADDR_MODE_X_INDEX_INDIRECT      = 0b0000000010000
ADDR_MODE_INDIRECT_Y_INDEX      = 0b0000000001000
ADDR_MODE_ZERO_PAGE             = 0b0000000000100
ADDR_MODE_ZERO_PAGE_X           = 0b0000000000010
ADDR_MODE_ZERO_PAGE_Y           = 0b0000000000001

def gen_bitfield_from_input():
    bitfield = 0
    mode = input(">> ")
    while (mode != "q"):
        if (mode == "acc"):
            bitfield |= ADDR_MODE_ACCUMULATOR
        elif (mode == "abs"):
            bitfield |= ADDR_MODE_ABSOLUTE
        elif (mode == "absx"):
            bitfield |= ADDR_MODE_ABSOLUTE_X
        elif (mode == "absy"):
            bitfield |= ADDR_MODE_ABSOLUTE_Y
        elif (mode == "imm"):
            bitfield |= ADDR_MODE_IMMEDIATE
        elif (mode == "imp"):
            bitfield |= ADDR_MODE_IMPLIED
        elif (mode == "ind"):
            bitfield |= ADDR_MODE_ABSOLUTE_INDIRECT
        elif (mode == "indx"):
            bitfield |= ADDR_MODE_X_INDEX_INDIRECT
        elif (mode == "indy"):
            bitfield |= ADDR_MODE_INDIRECT_Y_INDEX
        elif (mode == "rel"):
            bitfield |= ADDR_MODE_RELATIVE
        elif (mode == "zp"):
            bitfield |= ADDR_MODE_ZERO_PAGE
        elif (mode == "zpx"):
            bitfield |= ADDR_MODE_ZERO_PAGE_X
        elif (mode == "zpy"):
            bitfield |= ADDR_MODE_ZERO_PAGE_Y
        mode = input(">> ")

    print(f"0b{bitfield:013b}")

bitfields = {}
mnemonics = []
with open("../../../opcodes/6502ops.csv") as f:
    reader = csv.DictReader(f)

    for line in reader:
        if line["mnemonic"] not in mnemonics:
            mnemonics.append(line["mnemonic"])
    mnemonics = sorted(mnemonics)
    for instr in mnemonics:
        bitfields[instr] = 0
    f.seek(0)
    next(f)
    for line in reader:
        if (line["addressing mode"] == "ACC"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ACCUMULATOR
        elif (line["addressing mode"] == "ABS"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ABSOLUTE
        elif (line["addressing mode"] == "ABSX"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ABSOLUTE_X
        elif (line["addressing mode"] == "ABSY"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ABSOLUTE_Y
        elif (line["addressing mode"] == "IMM"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_IMMEDIATE
        elif (line["addressing mode"] == "IMP"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_IMPLIED
        elif (line["addressing mode"] == "IND"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ABSOLUTE_INDIRECT
        elif (line["addressing mode"] == "INDX"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_X_INDEX_INDIRECT
        elif (line["addressing mode"] == "INDY"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_INDIRECT_Y_INDEX
        elif (line["addressing mode"] == "REL"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_RELATIVE
        elif (line["addressing mode"] == "ZP"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ZERO_PAGE
        elif (line["addressing mode"] == "ZPX"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ZERO_PAGE_X
        elif (line["addressing mode"] == "ZPY"):
            bitfields[line["mnemonic"]] |= ADDR_MODE_ZERO_PAGE_Y

for mnemonic, mode in bitfields.items():
    print(f"#define {mnemonic:s}_BITFIELD 0b{mode:013b}")