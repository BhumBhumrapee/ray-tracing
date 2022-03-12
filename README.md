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

If compiled with openmp flag do this before starting the main program,

```bash
export OMP_NUM_THREADS=NUM_THREADS
```

Where NUM_THREADS depends on the system. For me, OMP_NUM_THREADS=NUM_16 yields the best performance for me. So,

```bash
export OMP_NUM_THREADS=16
```

After setting up the number of threads, we can run the program and redirect the input to the file we desire to store 
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


