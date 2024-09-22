from PIL import Image
from random import shuffle
from matplotlib.colors import rgb_to_hsv
import os
import numpy as np
import math


SIZE = 200
SUB = 3
SUB2 = SUB // 2


def histogram(mat):
    mat = mat[:,:,2]
    vals = []
    n,m = mat.shape
    for i in range(n):
        for j in range(m):
            valsub = mat[i-SUB2:i+SUB2+1, j-SUB2:j+SUB2+1].flatten('F')
            if len(valsub) == 0:
                continue
            vals.append(np.std(valsub))
    norm = len(vals)
    maxstd = max(vals)
    bins = [maxstd * i / SIZE for i in range(SIZE+1)]
    res = list(np.histogram(vals, bins=bins)[0] / norm * SIZE)
    res = [str(res[i]) for i in range(SIZE)]
    return res


def main():
    print('descriptor: texture histogram')
    f = open('texture_test.txt', 'w')
    test = open('tests.txt', 'w')
    file_list = os.listdir('./images/test')
    shuffle(file_list)
    nb = 0
    for file in file_list:
        if file[-3:] == 'jpg':
            nb += 1
    f.write(str(nb) + ' ' + str(SIZE)+'\n')
    for file in file_list:
        if file[-3:] != 'jpg':
            continue
        print('processing image:', file)
        img = Image.open('./images/test/'+file)
        img = np.array(img)
        img = rgb_to_hsv(img[:,:,:3])
        hist = histogram(img)         
        f.write(file.split('_')[0] + ' ')
        f.write(' '.join(hist))
        f.write('\n')
        test.write(file+'\n')
    f.close()


if __name__ == '__main__':
    main()
