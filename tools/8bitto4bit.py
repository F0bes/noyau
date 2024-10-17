import numpy as np
import sys

argnum = len(sys.argv)
if argnum != 3:
	print("Usage: python 8bitto4bit.py <inputfile> <outputfile>")
	sys.exit(1)

inputfile = sys.argv[1]
outputfile = sys.argv[2]

fb = open(inputfile, "rb")
array = np.fromfile(fb, dtype=np.uint8)

print(array)

array4b = (array[::2] & 0x0f) | (array[1::2] << 4)

print(array.size)

print(array4b.size)

array4b.tofile(outputfile)
