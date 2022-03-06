import numpy as np
from utils import *
width = 256
height = 256

ppm_header = f'P3\n{width} {height}\n255\n'

output_file_name = "output_image.ppm"

clear_image(output_file_name);
output_file = open(output_file_name, 'a')
output_file.write(ppm_header)
total_pixels = width * height

for j in range(height - 1, -1, -1):

    scaled_j = j * height
    print(f"Lines remaning: {j}")
    
    for i in range(width):

        total = scaled_j + i

        r = i / (width - 1)
        g = j / (height - 1)
        b = 0.25;
        
        ir = int(255.999 * r)
        ig = int(255.999 * g)
        ib = int(255.999 * b)  

        output_file.write(f"{ir} {ig} {ib} ")

    output_file.write("\n")

output_file.close()