import matplotlib.pyplot as plt
import numpy as np
f = open("ResultIdealPoints/result.txt","r")
points_start=int(f.readline())
points_step=int(f.readline())
noise_start=float(f.readline())
noise_step=float(f.readline())
magnitude = int(f.readline())  #for different magnitude
number_points = int(f.readline()) #for different number of image points
index_number = 0
index_magnitude = 0
list_number = []
list_magnitude = []
for i in range(0,magnitude):
    list_magnitude.append([])
for i in range(0,number_points):
    list_number.append([])
for line in f:
    if line == "*****\n":
        index_magnitude += 1
        index_number = 0
    else:
        list_magnitude[index_magnitude].append(float(line))
        list_number[index_number].append(float(line))
        index_number += 1
x1 = [] #same number of image points
for i in range(0,number_points):
    x1.append(points_start+i*points_step)
x2 = [] #same magnitude
for i in range(0,magnitude):
    x2.append(round(noise_start+i*noise_step,2))
plt.rcParams['figure.figsize']=(10.0,10.0)
plt.figure(1)
plt.subplots_adjust(wspace=0.5,hspace=0.5)
index = 1
for i in list_magnitude:
    plt.subplot(2,magnitude//2+1,index)
    plt.plot(x1,i,color='k',linestyle='-',marker='^',linewidth=1)
    plt.xlabel("Number of image points")
    plt.ylabel("Error")
    plt.title("M:"+str(x2[index-1]))
    index += 1
index = 1
plt.figure(2)
for i in list_number:
    plt.subplot(5,number_points//5+1,index)
    plt.plot(x2,i,color='k',linestyle='-',marker='^',linewidth=1)
    #plt.xlabel("Magnitude of noise")
    #plt.ylabel("Error")
    plt.title("N:"+str(x1[index-1]))
    index += 1
plt.subplots_adjust(wspace=0.5,hspace=0.5)
plt.figure(1).savefig("magnitude.png")
plt.figure(2).savefig("number.png")
plt.show()
