#include "hittable.h"
#include "vec3.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "rectangle.h"

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

hittable_list scene2() {

    hittable_list world;

    shared_ptr<lambertian> ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material)); // ground
    world.add(make_shared<sphere>(point3(0,0,-1005), 1000, ground_material)); // wall back
    //world.add(make_shared<sphere>(point3(1005, 0, 0), 1000, ground_material)); // wall right
    world.add(make_shared<sphere>(point3(0, 1005, 0), 1000, ground_material)); // top wall
    world.add(make_shared<sphere>(point3(0, 0, 1015), 1000, ground_material)); // wall front
    shared_ptr<light> light_mat = make_shared<light>(color(20, 20, 20));
    world.add(make_shared<sphere>(point3(-66, 4, 0), 50, light_mat));


    shared_ptr<material> material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 0.5, 4), 0.5, material1));
    shared_ptr<material> red_material = make_shared<lambertian>(color(1, 0, 0));
    world.add(make_shared<sphere>(point3(0, 2.5, 2), 0.5, red_material));
    shared_ptr<material> blue_material = make_shared<lambertian>(color(0, 0, 1));
    world.add(make_shared<sphere>(point3(-2, 0.5, 2), 0.5, blue_material));

    shared_ptr<material> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    world.add(make_shared<sphere>(point3(-3.5, 0.5, 1), 0.5, material1));

    shared_ptr<material> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    shared_ptr<material> green_material = make_shared<lambertian>(color(0, 1, 0));
    world.add(make_shared<sphere>(point3(4, 0.5, 2), 0.5, green_material));

    shared_ptr<material> metal_2 = make_shared<metal>(color(0.7, 0.3, 0.6), 0.0);
    world.add(make_shared<sphere>(point3(-2, 2, -2), 0.5, metal_2));

    shared_ptr<material> metal_3 = make_shared<metal>(color(0.132813, 0.09375, 0.78125), 0);
    world.add(make_shared<sphere>(point3(2, 0.5, 2), 0.5, metal_3));


    return world;
}

hittable_list scene3() {

    hittable_list world;
    shared_ptr<lambertian> ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    shared_ptr<material> metal_material = make_shared<lambertian>(color(0.4, 0.6, 0.5));
    shared_ptr<material> light_mat = make_shared<light>(color(4,4,4));
    world.add(make_shared<rectangle>(point3(1,0,0), point3(1,2,0), point3(3,0,0), metal_material));

    //world.add(make_shared<rectangle>(point3(-1,3,0), point3(-1,1,-1), point3(1,3,0), metal_material));

    // shared_ptr<material> metal_2 = make_shared<metal>(color(0.7, 0.3, 0.6), 0.0);
    // world.add(make_shared<sphere>(point3(0, 0.5, 0), 0.5, metal_2));

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

    // shared_ptr<material> light_mat = make_shared<light>(color(100,100,100));
    // world.add(make_shared<rectangle>(point3(-1,1.5,3), point3(-0.5,2,2.5), point3(-1.5,1.5,2.5), light_mat));

    return world;
}