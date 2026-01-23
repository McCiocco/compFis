import matplotlib.pyplot as plt
import matplotlib.patches as mpat
import numpy as np

fileName = "/home/ciocco/Desktop/projects/compFis/synchroPendulums/twoPends.txt" #name of the file

f = open(fileName, "r")

npoints = map(float, f.readline().strip().split(" "))

thetas1 = np.array(tuple(map(float, f.readline().strip().split(" "))))
thetas2 = np.array(tuple(map(float, f.readline().strip().split(" "))))

f.close()


# plt.plot(thetas1, 'r-', thetas2, 'b-')
plt.plot(thetas2 - thetas1)

plt.show()
