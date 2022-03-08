import math
import numpy as np
from hittable import hit_record, hittable
from ray import *

# short hand constants

pi = math.pi
infinity = math.inf

# utilities function

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

def ray_color(r: ray, world: hittable, depth):
    rec: hit_record = hit_record()

    if (depth <= 0):
        return np.array([0.0, 0, 0])

    if (world.hit(r, 0.001, infinity, rec)):
        target = rec.p + rec.normal + random_vector_in_unit_sphere()
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1)

    unit_direction = unit_vector(r.dir)
    t = 0.5 * (unit_direction[1] + 1)
    return (1 - t) * np.array([1,1,1]) + t * np.array([0.5, 0.7, 1])

def degrees_to_radians(degrees):
    return degrees * math.pi / 180

def random_vector():
    return np.array([np.random.rand(), np.random.rand(), np.random.rand()])

def random_vector(mn, mx):
    return np.array([np.random.uniform(mn, mx), np.random.uniform(mn, mx), np.random.uniform(mn, mx)])

def random_vector_in_unit_sphere():
    while True:
        p = random_vector(-1, 1)
        if np.dot(p, p) >= 1:
            continue;
        return p;