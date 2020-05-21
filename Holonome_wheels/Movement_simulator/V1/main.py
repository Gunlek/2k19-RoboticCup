import matplotlib.pyplot as plt

from Holonome_wheels.Movement_simulator.Robot import Robot

x = 60      # in cm
y = 80      # in cm
pos = (x, y)
a = lambda t: 5

robot = Robot((0, 0))

# distance:
#  d1 = d * sin(a - 90)
#  d2 = d * sin(a + 30)
#  d3 = d * sin(a + 150)

# vitesses:
#  v1 = v * sin(a - 90)
#  v2 = v * sin(a + 30)
#  v3 = v * sin(a + 150)

# Attention aux accélérations:
# En posant a(t) l'équation de l'accélération:
#  v1 = a(t) * v * sin(a - 90)
#  v2 = a(t) * v * sin(a + 30)
#  v3 = a(t) * v * sin(a + 150)

distance = robot.compute_distance_vector(pos)
