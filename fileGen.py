import sys
import numpy 

if len(sys.argv) < 2:
    print("Usage: python3 fileGen.py numFiles");
    exit()
fileSize = 64 * 1024 * 1024
numFiles = int(sys.argv[1])


# files for background processes
for i in range(numFiles):
	filename = str(i) + ".txt"
	fp = open(filename, "wb")
	fp.write(numpy.random.bytes(fileSize))
	fp.close()


# file for main benchmark
filename = "contention_test.txt"
fp = open(filename, "wb")
fp.write(numpy.random.bytes(fileSize))
fp.close()
