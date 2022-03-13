# Ray-tracing

A basic implementation of a ray-tracer in c++ (inititally in python).

## Project Directory

```
.
├── c++
│   └── images
│   └── src
├── python                   
├── .gitignore
└── README.md
```

## Compilation Guide

```bash
g++ main.cc -o main -fopenmp
```

Note: Add openmp flag to use openmp to speed up the rendering.

## Rendering

### Openmp settings

If **compiled with openmp flag** do this before starting the main program, else skip this section. 
To speed up the rendering process, I've used a little bit of parallel programming to do sampling in parallel using openmp.
Therefore, to speed up we have to set up the number of threads that openmp will be allowed to use to do work in parallel.
To set up this, use the following command,

```bash
export OMP_NUM_THREADS=NUM_THREADS
```

Where NUM_THREADS depends on the system and user's choice. For me, NUM_THREADS=16 yields the best performance for me. So,

```bash
export OMP_NUM_THREADS=16
```

Now we're ready to run the main program!

### Outputing image

Now we are ready to run the program. Run the main file and redirect the input to the file we desire to store 
the output in. To do this, use the following command,

```bash
./main > file_name.ppm 
```

Replace file_name with some file name. You can then open this file type in photoshop or other image viewer that support this file type.

## Featured Materials

There are, currently, a total of 4 materials which are the followings, along with the idea behind the implementation:

### 1: Lambterian material / Diffuse material

In reality, this type of material can be found on rough surfaces, be it visible to the eye or microscopically small bump. 
To simulate this behavior we can have the ray scattered in random direction when it hit the surface.

### 2: Metalic material

This is everyday metallic surface. Unlike diffuse surface which have a rough surface, metallic surface is smooth and thus the ray
doesn't get scattered in random direction but has a certain defined behavior to it. In fact this behaves like a mirror.

### 3: Dieletric material

This is all the clear material such as glass, water, and so on. Unlike those two that the ray get reflect / scattered, 
this time it also get refracted in addition to being reflected. 

### 4: Light material

This is for surface that emits light. So basically, this is used to generate light in a scene. To simulate this,
I let the ray that hit the light material returns light intensity which will be multiplied to the material's own color.

## Featured Shapes

There are currently a total of 2 shapes, which are the following:

### 1: Sphere

This shape is simple to implement as we can represent the surface in 3D and check for intersection 
with the ray quite easily.

### 2: Rectangle

A 2D rectangular surface. To define this surface we need two vectors which defines the length of two sides and also the orientation. 
To store this information, we can instead view the rectangle as a plane in 3D which then we can solve for the intersection quite easily.
To check if the intersection lies within the bound, we have serveral options. I thought of 2 ways which is first projection onto different planes, and 
second which i decided to implement which is using the property of dot products.

## Development

For most of the part, I've follow directly the instruction from the book. At first I've tried to implement everything in python, as it is much simpler
than c++ and that all of the utilities functions are already there in library such as numpy, scipy, and so on. At first, before all of the add-ons effect
to make the image look nicer, such as anti-aliasing, the code was running just fine at a good speed. However, after I've added anti-alising, 
the rendering process takes much longer time than expected, around 1 mininute for a simple image with 10 samples. Realising that this would we 
bad when we moved to thousands of samples, I've decided to move to c++ as the author of the book recommened. Another reason that I've chosen c++ is
the fact that I can gain access to multiprocessing library such as openmp very easily. This will be used to speed up the rendering process even more.

### Theory / Straight from the book

Like stated, most of the idea behind the implementation is followed directly from the book, this includes, camera, basic materials, hittable class abstraction, vector functions,
ray casting, sphere shape, and color. Some have little tweak in between but it is very minor.

### Extension to light material and rectangular shape

After finishing the book and having a proper running code, I've decided also to try to extend from the idea discussed in the book regarding material. I've tryied
to implement light material by myself. There was a lot of confusion in the first place, but the idea in the end was very simple. What I implemented was just to 
return the attenuation of the light without scattering the ray (as this is the source of the light, it should not scatter in my opinion). This attenuation
would then get multiply to the original color it bounces from.

```c++
virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
     attenuation = albedo; // return white color
     return false;
}
```

## NotesOn Reference Material

For most of the part, I've follow directly from the book called "Ray Tracing in One Weekend". With some vector utility functions 
being taken directly from the book in the c++ implementation version of my code. On some occasion I would deviate from the book
to try things on my own.

## Citation

[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

Title (series): “Ray Tracing in One Weekend Series”
Title (book): “Ray Tracing in One Weekend”
Author: Peter Shirley
Editors: Steve Hollasch, Trevor David Black
Version/Edition: v3.2.3
Date: 2020-12-07
URL (series): https://raytracing.github.io/
URL (book): https://raytracing.github.io/books/RayTracingInOneWeekend.html
