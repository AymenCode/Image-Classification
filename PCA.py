import numpy as np
from PIL import Image
from matplotlib.colors import rgb_to_hsv
import os

from random import shuffle


SIZE = 200
bins = [i/float(SIZE) for i in range(SIZE+1)]
n_components = 190
files = []

def image(name, colors):
    nb=0
    x=np.array([])
    file_list = os.listdir(name)
    shuffle(file_list)
    for file in file_list:
        if file[-3:] != 'jpg':
            continue
        
        nb += 1
        files.append(file)
        print('processing image:', file,'\t n°',nb)
        img = Image.open(name+file)
        if file[0] == '1' :
            colors.append('r')
        elif file[0] == '2' :
            colors.append('b')
        elif file[0] == '3' :
            colors.append('c')
        img = np.array(img)
        img = rgb_to_hsv(img[:,:,:3])
        vals = img[:,:,0].flatten('F')
        norm = len(vals)
        res = list(np.histogram(vals, bins=bins)[0] / norm * SIZE)
        x=np.concatenate((x,res))
        #res = [round(res[i]) for i in range(len(res))]
    return (nb,x)


colors=[]
nb,x=image('./images/',colors)
x=np.reshape(x, (nb,SIZE))
print(x.shape)
import matplotlib.pyplot as plt
plt.scatter(x[:,0],x[:,1],color=colors)
plt.xlabel("x1")
plt.ylabel("x2")
plt.show()

xbar=x.mean(axis=0)
print(xbar)

sigma=np.std(x,axis=0,ddof=1)
print(sigma)

z=(x-xbar)/sigma
print(z)

cov_mat=np.cov(x[:,:],rowvar=False)
print(cov_mat)

eigen_values , eigen_vectors = np.linalg.eigh(cov_mat)
print(eigen_values)
print(eigen_vectors)

sorted_index=np.argsort(eigen_values)[::-1]
print(sorted_index)

sorted_eigenvectors = eigen_vectors[:,sorted_index]
print(sorted_eigenvectors)

print(100*eigen_values[2]/sum(eigen_values))

n_components = 2
eigenvector_subset = sorted_eigenvectors[:,0:n_components]
print(eigenvector_subset)

transformed=np.matmul(x[:,:],eigenvector_subset)

print(transformed)
plt.scatter(transformed[:,0],transformed[:,1], color=colors)
plt.xlabel("x1")
plt.ylabel("x2")
plt.show()




colors1 = []
print('Testing!!!!!')
files = []
nb1,y=image('./images/test/',colors1)

y=np.reshape(y, (nb1,SIZE))

ybar=y.mean(axis=0)

sigma1=np.std(y,axis=0,ddof=1)

z1=(y-ybar)/sigma1

cov_mat1=np.cov(y[:,:],rowvar=False)
eigen_values1 , eigen_vectors1 = np.linalg.eigh(cov_mat1)

sorted_index1=np.argsort(eigen_values1)[::-1]

sorted_eigenvectors1 = eigen_vectors1[:,sorted_index1]

eigenvector_subset1 = sorted_eigenvectors1[:,0:n_components]

transformed1=np.matmul(y[:,:],eigenvector_subset1)
ncorrect = 0

fichier=open("guess1.txt","w")
fichier.write(str(nb1)+'\n')
for i in range(transformed1.shape[0]) :
    nr=0
    nb=0
    nc=0
    r=0
    b=0
    c=0
    for j in range (transformed.shape[0]) :
        if(colors[j]=='r') :
            nr+=1
            r+= sum([ (transformed[j,k]-transformed1[i,k]) * (transformed[j,k]-transformed1[i,k]) for k in range(n_components)])
        elif(colors[j]=='b') :
            nb+=1
            b+= sum([ (transformed[j,k]-transformed1[i,k]) * (transformed[j,k]-transformed1[i,k]) for k in range(n_components)])
        elif(colors[j]=='c') :
            nc+=1
            c+= sum([ (transformed[j,k]-transformed1[i,k]) * (transformed[j,k]-transformed1[i,k]) for k in range(n_components)])
    c/=nc
    r/=nr
    b/=nb
    mini=min([c,r,b])
    accur = 100*( (1/mini) / (1/c + 1/r + 1/b))
    if  mini==r:
        fichier.write(files[i]+ ' 1 ' + str(round(accur))+ '\n')
    elif  mini==b:
        fichier.write(files[i]+ ' 2 ' + str(round(accur))+ '\n')
    elif  mini==c:
        fichier.write(files[i]+ ' 3 ' + str(round(accur))+ '\n')
    
    if (colors1[i]=='c' and mini == c) or (colors1[i]=='b' and mini == b) or (colors1[i]=='r' and mini == r) :
        ncorrect+=1
       
print('precision : ' ,ncorrect/nb1)
fichier.close()
y=np.matmul(transformed,eigenvector_subset.transpose())
print(y)
