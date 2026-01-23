""" Produce a line plot of a set of points given from a file.
    File must be formatted as following:
    first row: lowx, highx, lowy, highy, npoints
    then Xdata (x0, x1, x2, ...) on a row
    ydata on the next
"""

import matplotlib.pyplot as plt
import matplotlib.patches as mpat
import numpy as np

fileName = "/home/ciocco/Desktop/projects/compFis/kepler/kepler.txt" #name of the file

f = open(fileName, "r")

lowx, highx, lowy, highy, npoints = map(float, f.readline().strip().split(" "))

rs = np.array(tuple(map(float, f.readline().strip().split(" ")))                                                                                )
thetas = np.array(tuple(map(float, f.readline().strip().split(" "))))
kinetics = np.array(tuple(map(float, f.readline().strip().split(" "))))
potentials = np.array(tuple(map(float, f.readline().strip().split(" "))))
totalEnergy = kinetics + potentials

f.close()

xs = rs * np.cos(thetas)
ys = rs * np.sin(thetas)


plt.subplot(121)
plt.title("Orbit")
ax = plt.gca()
ax.set_aspect('equal', adjustable='box')
plt.plot(xs, ys, 'bo', [0.0], [0.0], 'ro')

plt.subplot(122)
redPatch = mpat.Patch(color='red', label='Cinetica')
bluePatch = mpat.Patch(color='blue', label='Potenziale')
greenPatch = mpat.Patch(color='green', label='Totale')
plt.legend(handles=[redPatch, bluePatch, greenPatch])
plt.title("Energy")
plt.plot(kinetics, 'r-', potentials, 'b-', totalEnergy, 'g-')

plt.show()
