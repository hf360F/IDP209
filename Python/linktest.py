from http import server
from platform import python_branch
import socket
import random
from itertools import count
from sre_parse import State
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import numpy as np

HOST = "192.168.4.1"
PORT = 23

"""
Testing of live map updates and receiving state vector over WiFi.
Superceded by telmetry.py, now only useful for peformance testing of TCP socket connection.
"""

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setblocking(0)
s.settimeout(5)
s.connect((HOST, PORT))
s.send(b"Let me in") # The Arduino can't seem to recognise the connection until a client sends some bytes

ack = s.recv(64).decode("ASCII")
while ack != "New client connection": # Wait for connection
    ack = s.recv(64).decode("ASCII")
print("Connected to server.")
time.sleep(1)

IDList = []
vectors = []

try:
    while True:
        data = s.recv(128).decode("ASCII") # You may need to increase this if the state vector is extended
        try:
            dataSplit = data.split(" ")
            stateVector = {
                "stateID": dataSplit[1].split(",")[0], # State vector count
                "updateTime": dataSplit[1].split(",")[1], # Time in microseconds since last updateTime was sent
                "displacement": dataSplit[2].split(","), # List of x, y displacement
                "rotation": dataSplit[3], # Rotation in degrees
                "motorSpeeds": dataSplit[4].split(","), # List of left, right motor speed settings, 0-255 
                "motorPIDs": dataSplit[5].split(",") # List of P, I, D values
                }
            vectors.append(stateVector)
            IDList.append(stateVector["stateID"])
            if len(IDList) > 1:
                print(int(IDList[-1]) == int(old)+1, int(IDList[-1]))
                if (int(IDList[-1]) != int(old)+1):
                    print(IDList)
                    print(vectors[-5:])
                    print(f"Got {int(IDList[-1])}, expected {(int(old) + 1)}")
                    raise Exception(f"Failed after {len(IDList)} reads")
            old = IDList[-1]
        except IndexError:
            print("Attempted index into state vector failed") # Occurs when "data" cannot be interpreted as a valid robot state vector
except KeyboardInterrupt:
    pass


"""
plt.style.use('fivethirtyeight')

x_vals = [1200]
y_vals = [1200]

index = count()

img = plt.imread("Arena.png")
fig, ax = plt.subplots(figsize=(12, 12))
ax.set_xlim(0, 2400)
ax.set_ylim(0, 2400)
ax.imshow(img, extent=(0, 2400, 0, 2400))

plt.grid(False)
plt.axis("off")

line = ax.plot(x_vals, y_vals)[0]

def animate(i):
    x_vals.append(x_vals[-1]+random.randint(-50, 51))
    y_vals.append(y_vals[-1]+random.randint(-50, 51))
    line.set_xdata(x_vals)
    line.set_ydata(y_vals)
    fig.tight_layout()


ani = FuncAnimation(fig, animate, interval=100)

plt.draw()
plt.show()
"""