import matplotlib.pyplot as plt
import numpy as np
import sys
store=sys.argv[1]
file_name="ResultIdealPoints/"+store
f = open(file_name+"/result.txt","r")
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
plt.figure(1).savefig(file_name+"/magnitude.png")
plt.figure(2).savefig(file_name+"/number.png")
for i in range(0,magnitude):
    l=open(file_name+"/noise"+str(i)+".txt")
    index=int(l.readline())
    number_of_points=int(l.readline())
    number_of_images=0
    X=[]
    Y=[]
    x1=list(range(1,1+number_of_points))
    list_x=[]
    list_y=[]
    for line in l:
        if line != "******\n":
            if len(list_x) == len(list_y):
                list_x.append(float(line))
            else:
                list_y.append(float(line))
        else:
            number_of_images+=1
            X.append(list_x)
            Y.append(list_y)
            list_x=[]
            list_y=[]
    plt.rcParams['figure.figsize']=(10.0,10.0)
    plt.figure(3+i)
    plt.title("Noise"+str(noise_start+i*noise_step))
    plt.subplot(1,2,1)
    for index in range(number_of_images):
        plt.plot(x1,X[index],color='k')
    plt.xlabel("Points_x")
    plt.ylabel("noise")
    plt.subplot(1,2,2)
    for index in range(number_of_images):
        plt.plot(x1,Y[index],color='k')
    plt.xlabel("Points_y")
    plt.ylabel("noise")
    plt.figure(3+i).savefig(file_name+"/noise"+str(i)+".png")
plt.show()
