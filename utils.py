import math
import sqlite3
import numpy as np
from ray import *

def clear_image(filename):
    f = open(filename, 'w')
    f.write("")
    f.close()

def write_color(file, color):
    file.write(f"{int(color[0] * 255.999)} {int(color[1] * 255.999)} {int(color[2] * 255.999)} ")

def unit_vector(vec):
    return vec / np.linalg.norm(vec)

def ray_color(r: ray):
    t = hit_sphere(np.array([0,0,-1]), 0.5, r)
    if (t > 0):
        n = unit_vector(r.at(t) - np.array([0, 0, -1]))
        return 0.5 * (n + np.array([1,1,1]))
    unit_direction = unit_vector(r.dir)
    t = 0.5 * (unit_direction[1] + 1)
    return (1 - t) * np.array([1,1,1]) + t * np.array([0.5, 0.7, 1])

def hit_sphere(center, radius, r: ray):
    oc = r.origin - center;
    a = np.dot(r.dir, r.dir)
    half_b = np.dot(oc, r.dir)
    c = np.dot(oc, oc) - radius ** 2
    discriminant = half_b ** 2 - a*c;
    if (discriminant < 0):
        return -1
    else:
        return (-half_b - math.sqrt(discriminant)) / a



