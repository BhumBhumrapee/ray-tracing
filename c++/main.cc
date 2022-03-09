#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "omp.h"
#include <iostream>
#include <time.h>

color ray_color(const ray& r, const hittable& world, int depth) {
    
    hit_record rec;

    if (depth <= 0) {
        return color(0,0,0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return attenuation;
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5* (unit_direction.y() + 1.0);
    return (1 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1);
}

hittable_list scene1() {

    hittable_list world;

    shared_ptr<material> ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    shared_ptr<material> material1 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    shared_ptr<material> material2 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 3, 0), 1.0, material2));

    shared_ptr<material> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 5, 0), 1.0, material3));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(0.6);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    return world;

}

hittable_list random_scene() {
    hittable_list world;

    shared_ptr<lambertian> ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(0.6);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<material> material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    shared_ptr<material> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    shared_ptr<material> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));


    return world;
}

int main() {

    // Settings

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 32;
    const int max_depth = 8;

    // World

    hittable_list world = scene1();

    point3 lookfrom(0,1,10);
    point3 lookat(0,3,0);
    vec3 vup(0,0,1);
    double dist_to_focus = 10;
    double aperture = 0;


    camera cam(lookfrom, lookat, vup, 50, aspect_ratio, aperture, dist_to_focus);

    /*
    point3 lookfrom(13,2,6);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0.5);
    double dist_to_focus = 10.0;
    double aperture = 0.1;


    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    */

    // Render

    time_t my_time = time(NULL);
    std::cerr << "init time: " << ctime(&my_time) << "\n";
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {

        std::cerr << "\rlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {

            color pixel_color;
            color partial_color;

            // uses 16 threads
            #pragma omp parallel private(partial_color) shared(pixel_color) // parallelization
            {
                
                pixel_color = color(0,0,0);
                partial_color = color(0,0,0);

                #pragma omp for 
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    partial_color += ray_color(r, world, max_depth);
                }   

                #pragma omp critical 
                pixel_color += partial_color;

            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "finish time: " << ctime(&my_time) << "\n";
    std::cerr << "\nDone.\n";
}
