import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib.colors as mcolors
import numpy as np
from matplotlib.animation import FuncAnimation, PillowWriter
from mpl_toolkits.mplot3d import Axes3D 
import matplotlib; matplotlib.use("TkAgg")
import csv
import configparser
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import random

config = configparser.ConfigParser()
config.read_file(open(r'config.ini'))

FILENAME = 'build/pos.csv'
FILENAME2 = 'output/mass.csv'
BOUNDARY = float(config.get('config', 'BOUNDARY'))
mass_maximum = float(config.get('config', 'mass_maximum'))
mass_minimum = float(config.get('config', 'mass_minimum'))
positions = []

class Point:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

def grouped(iterable, n):
    return zip(*[iter(iterable)]*n)


def init():
    size_factor = 1.5
    ax.set(xlim3d=(-size_factor*BOUNDARY, size_factor*BOUNDARY), xlabel='X')
    ax.set(ylim3d=(-size_factor*BOUNDARY, size_factor*BOUNDARY), ylabel='Y')
    ax.set(zlim3d=(-size_factor*BOUNDARY, size_factor*BOUNDARY), zlabel='Z')
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False
    
    ax.xaxis.pane.set_edgecolor('black')
    ax.yaxis.pane.set_edgecolor('black')
    ax.zaxis.pane.set_edgecolor('black')
    ax.set_facecolor('white')
    ax.grid(True)
    
    ax.xaxis.label.set_color('black')        
    ax.yaxis.label.set_color('black')        
    ax.zaxis.label.set_color('black')

    ax.tick_params(axis='x', colors='black')    
    ax.tick_params(axis='y', colors='black')    
    ax.tick_params(axis='z', colors='black')

    ax.spines['left'].set_color('black')
    ax.spines['top'].set_color('black')


def update_graph(frame):
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
    graph._offsets3d = (x, y, z)

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
    
with open(FILENAME2, newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    masses = [float(mass) for mass in next(reader) if mass.strip()]

mass_scale = 20
normalized_masses = [((mass - mass_minimum) / (mass_maximum - mass_minimum))*mass_scale for mass in masses]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

initial_positions = positions[0]
x = [pos.x for pos in initial_positions]
y = [pos.y for pos in initial_positions]
z = [pos.z for pos in initial_positions]


graph = ax.scatter(x, y, z, c='purple', marker='o', s=normalized_masses)

ani = matplotlib.animation.FuncAnimation(fig, update_graph, init_func=init, interval=3, blit=False, cache_frame_data=False)

plt.show()