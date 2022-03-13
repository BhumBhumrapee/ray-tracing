#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "utils.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale); //sqrt for gamma correction, gamma 2
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    out << int(256 * clamp(r, 0.0, 0.999)) << ' ' // ensure the the written color is between 0 and 256.
        << int(256 * clamp(g, 0.0, 0.999)) << ' '
        << int(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif