

from wsgiref.simple_server import WSGIRequestHandler
import numpy as np
from utils import *
from ray import *
from hittable import *
from sphere import *
from PIL import Image
import cv2 as cv

# settings

# image

aspect_ratio = 16 / 9
image_width = 400
image_height = int(image_width / aspect_ratio)

# World

world: hittable_list = hittable_list()
world.add(sphere([0,0,-1], 0.5))
world.add(sphere([0, -100.5, -1], 100))

# Camera

viewport_height = 2
viewport_width = aspect_ratio * viewport_height
focal_length = 1

origin = np.array([0,0,0])
horizontal = np.array([viewport_width, 0, 0])
vertical = np.array([0, viewport_height, 0])
lower_left_corner = origin - horizontal/2 - vertical/2 - np.array([0, 0, focal_length])

# Rendering

ppm_header = f'P3\n{image_width} {image_height}\n255\n'

output_file_name = "output_image.ppm"

clear_image(output_file_name);
output_file = open(output_file_name, 'a')
output_file.write(ppm_header)
total_pixels = image_width * image_height

for j in range(image_height - 1, -1, -1):

    print(f"Lines remaning: {j}")

    for i in range(image_width):

        u = i / (image_width - 1)
        v = j / (image_height - 1)
        
        r:ray = ray(origin, lower_left_corner + u * horizontal + v * vertical - origin)
        color = ray_color(r, world)
        write_color(output_file, color)

    output_file.write("\n")

output_file.close()

im = cv.imread(output_file_name)
cv.imwrite("output_image_png.png", im)
