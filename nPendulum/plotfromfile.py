import matplotlib.pyplot as plt
import matplotlib.patches as mpat
import numpy as np

fileName = "/home/ciocco/Desktop/projects/compFis/nPendulum/nPendulum.txt" #name of the file

f = open(fileName, "r")

N, npoints = map(float, f.readline().strip().split(" "))

thetas = [np.array(tuple(map(float, line.strip().split(" ")))) for line in f.readlines()]

f.close()

for i, arr in enumerate(thetas):
    plt.plot(arr, label=f'Theta {i+1}')

plt.title(f"{int(N)}-Pendolo")



plt.show()
