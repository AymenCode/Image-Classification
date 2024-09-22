from random import shuffle
from copy import deepcopy


f = open('color.txt','r')
lines = deepcopy(f.readlines())
first_line = lines[0]
rest = lines[1:]
shuffle(rest)
f.close()
f = open('color.txt','w')
new_file = "".join(rest)
f.write(first_line + '\n')
f.write(new_file)
f.close()
