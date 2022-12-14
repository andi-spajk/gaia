def djb2(string):
    hash_val = 5381
    for ch in string:
        hash_val = ((hash_val << 5) + hash_val) + ord(ch)
        # hash_val &= 0x00000000FFFFFFFF
    return hash_val

def apply_hashes():
    size = 17
    with open("labels.txt", "r") as f:
        lines = f.readlines()
        for i, line in enumerate(lines):
            # strip newline
            if line[len(line) - 1] == '\n':
                line = line[0:len(line) - 1]

            hash_val = djb2(line) % size
            print(f"{i+1:d}\t{line:s}\t{hash_val:d}")

if __name__ == "__main__":
    apply_hashes()
