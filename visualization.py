import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
import matplotlib; matplotlib.use("TkAgg")
import csv

FILENAME = 'build/pos.csv'
BOUNDARY = 1000000
positions = []

class Point:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

def grouped(iterable, n):
    return zip(*[iter(iterable)]*n)

with open(FILENAME, newline='') as csvfile:

    reader = csv.reader(csvfile, delimiter=',')
    initial_positions = []
    positions = []
    first_row = next(reader)
    for (x, y, z) in grouped(first_row, 3):
        initial_positions.append(Point(x, y, z))
    for row in reader:
        positions.append([])
        for (x, y, z) in grouped(row, 3):
            positions[-1].append(Point(x, y, z))

    # for i in range(len(positions)):
    #     for j in range(len(positions[0])):
    #         positions[i][j].x += initial_positions[j].x
    #         positions[i][j].y += initial_positions[j].y
    #         positions[i][j].z += initial_positions[j].z
    positions = np.array(positions)


def iteration(frame):
    if frame < len(positions):
        pos = positions[frame]
    else:
        pos = positions[-1]
    x = []
    y = []
    for body in pos:
        x.append(body.x)
        y.append(body.y)
    ln.set_data(x, y)
    return ln,

def init():
   ax.set_xlim(-BOUNDARY, BOUNDARY)
   ax.set_ylim(-BOUNDARY, BOUNDARY)
   return ln,


fig, ax = plt.subplots()
xdata, ydata = [], []
ln, = plt.plot([], [], 'o')

ani = FuncAnimation(fig, iteration, interval = 1, init_func=init, blit=True)
plt.show()
