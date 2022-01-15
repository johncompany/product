#!/usr/bin/python3

import sys

#
# Entry point
#
if len(sys.argv) < 8:
    # CodeStartAddr <= Code < CodeEndAddr
    print("Usage: {0} HexFileName CodeHexFileName CodeStartAddr CodeEndAddr DataHexFileName DataStartAddr DataEndAddr[ Size ]".format(sys.argv[0]))

hexFileName = sys.argv[1]
codeHexFileName = sys.argv[2]
codeStartAddr = int(sys.argv[3], base=16)
codeEndAddr = int(sys.argv[4], base=16)
dataHexFileName = sys.argv[5]
dataStartAddr = int(sys.argv[6], base=16)
dataEndAddr = int(sys.argv[7], base=16)

code = []
data = []
zero = "00000000\n"

with open(hexFileName, "r") as hexFile, open(codeHexFileName, "w") as codeHexFile, open(dataHexFileName, "w") as dataHexFile:
    for i, l in enumerate(hexFile):
        addr = i * 4
        if codeStartAddr <= addr < codeEndAddr:
            codeHexFile.write(l)
        else:
            codeHexFile.write(zero)
        
        if dataStartAddr <= addr < dataEndAddr:
            dataHexFile.write(l)
        else:
            dataHexFile.write(zero)
