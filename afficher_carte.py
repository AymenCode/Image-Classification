import numpy as np 
from PIL import Image

f = open('carte_cat.txt', 'r')
L = f.readlines()

TAILLE = int(L[0])

im = Image.new("RGB", (TAILLE, TAILLE), "black")
pix = im.load()
for x in range(TAILLE):
    line = L[x+1].strip('\n').split(' ')
    for y in range(TAILLE):
        if line[y] == "0":
            pix[x, y] = (255, 255, 255)
        if line[y] == "1":
            pix[x, y] = (255, 0, 0)
        if line[y] == "2":
            pix[x, y] = (0, 255, 0)
        if line[y] == "3":
            pix[x, y] = (0, 0, 255)

im = im.resize((500, 500), resample=Image.NEAREST)
im.show()
im.save("test.png", "PNG")
