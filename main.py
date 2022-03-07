

from wsgiref.simple_server import WSGIRequestHandler
import numpy as np
from camera import camera
from utils import *
from ray import *
from hittable import *
from sphere import *
import cv2 as cv

# settings

# image

aspect_ratio = 16 / 9
image_width = 400
image_height = int(image_width / aspect_ratio)
samples_per_pixel = 100

# World

world: hittable_list = hittable_list()
world.add(sphere([0,0,-1], 0.5))
world.add(sphere([0, -100.5, -1], 100))

# Camera

cam: camera = camera()

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

        pixel_color = np.array([0.0,0,0])
        
        for s in range(samples_per_pixel):
            u = (i + np.random.rand()) / (image_width - 1)
            v = (j + np.random.rand()) / (image_height - 1)

            r = cam.get_ray(u, v)
            pixel_color += ray_color(r, world)

        write_color(output_file, pixel_color, samples_per_pixel)
    output_file.write("\n")

output_file.close()

im = cv.imread(output_file_name)
cv.imwrite("output_image_png.png", im)
