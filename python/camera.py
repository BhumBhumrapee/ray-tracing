from pkgutil import ImpImporter
import numpy as np
from ray import *

class camera:
    
    def __init__(self):
        self.aspect_ratio = 16 / 9
        self.viewport_height = 2
        self.viewport_width = self.aspect_ratio * self.viewport_height
        self.focal_length = 1

        self.origin = np.array([0,0,0])
        self.horizontal = np.array([self.viewport_width, 0, 0])
        self.vertical = np.array([0, self.viewport_height, 0])
        self.lower_left_corner = self.origin - self.horizontal/2 - self.vertical/2 - np.array([0, 0, self.focal_length])

    def get_ray(self, u:float, v: float) -> ray:
        return ray(self.origin, self.lower_left_corner + u * self.horizontal + v * self.vertical - self.origin)
