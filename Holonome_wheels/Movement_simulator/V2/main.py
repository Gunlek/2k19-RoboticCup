import math
import matplotlib.pyplot as plt
import numpy as np

def scalar(A, B):
    return A[0][0] * B[0][0] + A[1][0] * B[1][0]

def rad2deg(angle):
    return angle * 180 / math.pi

# The base you're working on
vector_base_x = np.array([[1], [0]])
vector_base_y = np.array([[0], [1]])

# The current position of the robot
curr_x = 0
curr_y = 0

# The desired destination of the robot
dest_x = 50     # In mm
dest_y = 50     # in mm

# The vector that describe the move you're trying to achieve
move_vector = np.array([[dest_x - curr_x], [dest_y - curr_y]])

norm_move_vector = np.linalg.norm(move_vector)
normalized_move_vector = 0 if norm_move_vector == 0 else move_vector / norm_move_vector
scalar_product = scalar(normalized_move_vector, vector_base_x)

# Theta and distance for v1, v2, v3 computing
theta = math.acos(scalar_product)
distance = norm_move_vector

# Constant, speed of robot
V = 10              # In mm/s
wheel_radius = 50   # in mm

v1 = V * math.sin(theta - 90)
v2 = V * math.sin(theta + 30)
v3 = V * math.sin(theta + 150)

d1 = distance * math.sin(theta - 90)
d2 = distance * math.sin(theta + 30)
d3 = distance * math.sin(theta + 150)

N1 = v1 / wheel_radius * 60     # In RPM
N2 = v2 / wheel_radius * 60     # In RPM
N3 = v3 / wheel_radius * 60     # In RPM

N1_elec = N1 * 2 * math.pi / 60 * 7     # In electrical rad/s
N2_elec = N2 * 2 * math.pi / 60 * 7     # In electrical rad/s
N3_elec = N3 * 2 * math.pi / 60 * 7     # In electrical rad/s

print("Speeds: ", v1, v2, v3)
print("Distances: ", d1, d2, d3)
print("Durations:", d1/v1, d2/v2, d3/v3)
print("Rotations:", N1, N2, N3)
print("Elec. rotations:", N1_elec, N2_elec, N3_elec)