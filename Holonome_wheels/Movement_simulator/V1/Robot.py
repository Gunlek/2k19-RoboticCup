import math


class Robot:

    def __init__(self, pos):
        self.x = pos[0]
        self.y = pos[1]
        self.max_speed = 10     # max speed in cm/s for the robot

    def compute_distance(self, to_dest):
        return math.sqrt((to_dest[0] - self.x)**2 + (to_dest[1] - self.y)**2)

    def compute_angle(self, to_dest):
        scalar_dot = self.x * to_dest[0] + self.y * to_dest[1]
        return math.acos(scalar_dot) * 180 / math.pi

    def compute_distance_vector(self, to_dest):
        distance = self.compute_distance(to_dest)
        angle = self.compute_angle(to_dest)
        d1 = distance * math.sin(angle - 90)
        d2 = distance * math.sin(angle + 30)
        d3 = distance * math.sin(angle + 150)

        return d1, d2, d3

    def compute_speed_vector(self, to_dest):
        angle = self.compute_angle(to_dest)
        v1 = self.max_speed * math.sin(angle - 90)
        v2 = self.max_speed * math.sin(angle + 30)
        v3 = self.max_speed * math.sin(angle + 150)
        return v1, v2, v3

    # TODO: We should add a function to compute the amount of time we have to accelerate or at least move
