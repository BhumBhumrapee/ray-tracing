import math
import sqlite3
import numpy as np
from hittable import hit_record, hittable
from ray import *

pi = math.pi
infinity = math.inf

def clamp(x: float, mn: float, mx: float):
    if (x < mn):
        return mn
    if (x > mx):
        return mx
    return x

def clear_image(filename):
    f = open(filename, 'w')
    f.write("")
    f.close()

def write_color(file, pixel_color, sample_per_pixel: int):
    r = pixel_color[0]
    g = pixel_color[1]
    b = pixel_color[2]

    scale = 1 / sample_per_pixel
    r *= scale
    g *= scale
    b *= scale

    file.write(f"{int(256 * clamp(r, 0, 0.999))} {int(256 * clamp(g, 0, 0.999))} {int(256 * clamp(b, 0, 0.999))} ")

def unit_vector(vec):
    return vec / np.linalg.norm(vec)

def ray_color(r: ray, world: hittable):
    rec: hit_record = hit_record()

    if (world.hit(r, 0, infinity, rec)):
        return 0.5 * (rec.normal + np.array([1,1,1]))
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

def degrees_to_radians(degrees):
    return degrees * math.pi / 180

