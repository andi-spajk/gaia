"""

generate dictionary of string-list pairs
string is the mnemonic, list contains 2-long tuples
tuples pair valid addressing mode with the opcode
ex. {"ADC":[(0x69,IMM),(0x65,ZP),(0x75,ZPX)]}

int assemble_ADC(int16_t bitflag)
{
\tswitch(bitflag) {
\tcase ADDR_MODE_IMMEDIATE: return 0x69;
\tcase ADDR_MODE_ZERO_PAGE: return 0x65;
\tcase ADDR_MODE_ZERO_PAGE_X: return 0x75;
\tcase ADDR_MODE_ABSOLUTE: return 0x6D;
\tcase ADDR_MODE_ABSOLUTE_X: return 0x7D;
\tcase ADDR_MODE_ABSOLUTE_Y: return 0x79;
\tcase ADDR_MODE_X_INDEXED_INDIRECT: return 0x61;
\tcase ADDR_MODE_INDIRECT_Y_INDEXED: return 0x71;
\tdefault: return ERROR_ILLEGAL_ADDRESSING_MODE
\t}
}

generate switch statement for each mnemonic
for mnemonic,list in dictionary
    print "int assemble_%s(int16_t bitflag)", mnemonic
    print "{"
    print "\tswitch(bitflag) {"
    for each tuple in list
        opcode = first tuple element
        addr mode = 2nd tuple element
        print "\tcase "
        if addr mode == "IMM"
            print "ADDR_MODE_IMMEDIATE: return "
            print "%x;", opcode
        etc.
    print "\tdefault: return ERROR_ILLEGAL_ADDRESSING_MODE"
    print "\t}"
    print "}"

"""

import csv

bitfields = {}
with open("../../../opcodes/6502ops.csv") as f:
    reader = csv.DictReader(f)

    mnemonics = []
    for line in reader:
        if line["mnemonic"] not in mnemonics:
            mnemonics.append(line["mnemonic"])

    mnemonics = sorted(mnemonics)
    for i in mnemonics:
        bitfields[i] = []

    f.seek(0)
    next(f)
    for line in reader:
        opcode = int(line["opcode"], 16)
        addr_mode = line["addressing mode"]
        pair = (opcode, addr_mode)
        # print(f"(0x{pair[0]:x}, {pair[1]:s})")
        bitfields[line["mnemonic"]].append(pair)

def gen_functions():
    for instr,pairs in bitfields.items():
        print(f"int assemble_{instr:s}(int16_t bitflag)")
        print("{")
        print("\tswitch (bitflag) {")
        for pair in pairs:
            opcode = pair[0]
            addr_mode = pair[1]
            print("\tcase ", end='')
            if (addr_mode == "ACC"):
                print(f"ADDR_MODE_ACCUMULATOR: return 0x{opcode:x};")
            elif (addr_mode == "ABS"):
                print(f"ADDR_MODE_ABSOLUTE: return 0x{opcode:x};")
            elif (addr_mode == "ABSX"):
                print(f"ADDR_MODE_ABSOLUTE_X: return 0x{opcode:x};")
            elif (addr_mode == "ABSY"):
                print(f"ADDR_MODE_ABSOLUTE_Y: return 0x{opcode:x};")
            elif (addr_mode == "IMM"):
                print(f"ADDR_MODE_IMMEDIATE: return 0x{opcode:x};")
            elif (addr_mode == "IMP"):
                print(f"ADDR_MODE_IMPLIED: return 0x{opcode:x};")
            elif (addr_mode == "IND"):
                print(f"ADDR_MODE_INDIRECT: return 0x{opcode:x};")
            elif (addr_mode == "INDX"):
                print(f"ADDR_MODE_X_INDEXED_INDIRECT: return 0x{opcode:x};")
            elif (addr_mode == "INDY"):
                print(f"ADDR_MODE_INDIRECT_Y_INDEXED: return 0x{opcode:x};")
            elif (addr_mode == "REL"):
                print(f"ADDR_MODE_RELATIVE: return 0x{opcode:x};")
            elif (addr_mode == "ZP"):
                print(f"ADDR_MODE_ZERO_PAGE: return 0x{opcode:x};")
            elif (addr_mode == "ZPX"):
                print(f"ADDR_MODE_ZERO_PAGE_X: return 0x{opcode:x};")
            elif (addr_mode == "ZPY"):
                print(f"ADDR_MODE_ZERO_PAGE_Y: return 0x{opcode:x};")
        print("\tdefault: return ERROR_ILLEGAL_ADDRESSING_MODE;")
        print("\t}")
        print("}")
        print()

def gen_headers():
    for instr,pairs in bitfields.items():
        print(f"int assemble_{instr:s}(int16_t bitflag);")

if __name__ == "__main__":
    gen_functions()
    # gen_headers()