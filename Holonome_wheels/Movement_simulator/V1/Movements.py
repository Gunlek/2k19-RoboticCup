class Move:

    wheel_distances = [0, 0, 0]
    time = 0
    start = (0, 0)
    destination = (0, 0)

    def __init__(self, start_point, destination_point, distance_vector, speed_vector, acceleration_law):
        self.start = start_point
        self.destination = destination_point
        self.distance_vector = distance_vector
        self.speed_vector = speed_vector
        self.acceleration_law = acceleration_law

    def compute_position(self, delta_t):
        self.time += delta_t
        wheel_1_speed = self.speed_vector[0] * self.acceleration_law(self.time)
        wheel_2_speed = self.speed_vector[2] * self.acceleration_law(self.time)
        wheel_3_speed = self.speed_vector[3] * self.acceleration_law(self.time)

        self.wheel_distances[0] += wheel_1_speed * delta_t
        self.wheel_distances[1] += wheel_2_speed * delta_t
        self.wheel_distances[2] += wheel_3_speed * delta_t
