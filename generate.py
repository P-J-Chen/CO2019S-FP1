import random
import sys

lengths=[]
total=15
f= open("data","w+")
for i in range(total):
    length=random.randint(5, 20)
    lengths.append(4*length)
    for j in range(0,length):
        f.write("/x" + hex(random.randint(0, 256))[2:].zfill(2))
    f.write("\n")
f.close()

f= open("lengths","w+")
for i in range(total):
    f.write(str(lengths[i])+"\n")
f.close()
