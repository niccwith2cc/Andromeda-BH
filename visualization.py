import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation, PillowWriter 
import matplotlib; matplotlib.use("TkAgg")
import csv

FILENAME = 'pos.csv'
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
    positions = np.array(positions)


def iteration(frame):
    if frame < len(positions):
        pos = positions[frame]
    else:
        pos = positions[-1]
    x = []
    y = []
    z = []
    for body in pos:
        x.append(body.x)
        y.append(body.y)
        z.append(body.z)
    ln.set_data_3d(x, y, z)
    return ln,

def init():
    ax.set(xlim3d=(-BOUNDARY, BOUNDARY), xlabel='X')
    ax.set(ylim3d=(-BOUNDARY, BOUNDARY), ylabel='Y')
    ax.set(zlim3d=(-BOUNDARY, BOUNDARY), zlabel='Z')
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False
    
    ax.xaxis.pane.set_edgecolor('w')
    ax.yaxis.pane.set_edgecolor('w')
    ax.zaxis.pane.set_edgecolor('w')
    ax.set_facecolor('black')
    ax.grid(False)
    
    ax.xaxis.label.set_color('white')        
    ax.yaxis.label.set_color('white')        
    ax.zaxis.label.set_color('white')

    ax.tick_params(axis='x', colors='white')    
    ax.tick_params(axis='y', colors='white')    
    ax.tick_params(axis='z', colors='white')

    ax.spines['left'].set_color('white')
    ax.spines['top'].set_color('white')
    return ln,


#fig, ax = plt.subplots()
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
xdata, ydata, zdata = [], [], []
ln, = ax.plot([], [], [], "o")

ani = FuncAnimation(fig, iteration, interval = 1, init_func=init, blit=True)
plt.show()
