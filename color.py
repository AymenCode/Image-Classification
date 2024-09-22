from PIL import Image
from random import shuffle
from matplotlib.colors import rgb_to_hsv
import numpy as np
import os


SIZE = 200
bins = [i/float(SIZE) for i in range(SIZE+1)]


def histogram(mat):
    vals = mat[:,:,0].flatten('F')
    norm = len(vals)
    res = list(np.histogram(vals, bins=bins)[0] / norm * SIZE)
    res = [str(res[i]) for i in range(SIZE)]
    return res


def main():
    print('descriptor: hue histogram')
    f = open('color.txt', 'w')
    file_list = os.listdir('./images')
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
        img = Image.open('./images/'+file)
        img = np.array(img)
        img = rgb_to_hsv(img[:,:,:3])
        hist = histogram(img)        
        f.write(file.split('_')[0] + ' ')
        f.write(' '.join(hist))
        f.write('\n')
    f.close()


if __name__ == '__main__':
    main()
