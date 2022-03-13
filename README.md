# Ray-tracing

A basic implementation of a ray-tracer in c++ (initially in python).

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

Note: Add OpenMP flag to use OpenMP to speed up the rendering.

## Rendering

### Openmp settings

If **compiled with OpenMP flag** do this before starting the main program, else skip this section. 
To speed up the rendering process, I've used a little bit of parallel programming to do sampling in parallel using OpenMP.
Therefore, to speed up we have to set up the number of threads that OpenMP will be allowed to use to do work in parallel.
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

Replace file_name with some file name. You can then open this file type in photoshop or other image viewers that support this file type.

## Featured Materials

There are, currently, a total of 4 materials which are the followings, along with the idea behind the implementation:

### 1: Lambterian material / Diffuse material

In reality, this type of material can be found on rough surfaces, be it visible to the eye or microscopically small bump. 
To simulate this behaviour we can have the ray scattered in random directions when it hit the surface.

### 2: Metalic material

This is an everyday metallic surface. Unlike diffuse surfaces which have a rough surface, the metallic surface is smooth and thus the ray
doesn't get scattered in random direction but has a certain defined behaviour to it. In fact, this behaves like a mirror.

### 3: Dieletric material

This is all the clear material such as glass, water, and so on. Unlike those two that the ray get reflect/scattered, 
this time it also gets refracted in addition to being reflected. 

### 4: Light material

This is for a surface that emits light. So basically, this is used to generate light in a scene. To simulate this,
I let the ray that hit the light material returns light intensity which will be multiplied by the material's own colour.

## Featured Shapes

There are currently a total of 2 shapes, which are the following:

### 1: Sphere

This shape is simple to implement as we can represent the surface in 3D and check for intersection 
with the ray quite easily.

### 2: Rectangle

A 2D rectangular surface. To define this surface we need two vectors that define the length of two sides and also the orientation. 
To store this information, we can instead view the rectangle as a plane in 3D which then we can solve for the intersection quite easily.
To check if the intersection lies within the bound, we have several options. I thought of 2 ways which are first projection onto different planes, and 
the second which I decided to implement is using the property of dot products.

## Development

For most of the part, I've followed directly the instruction from the book. At first, I've tried to implement everything in python, as it is much simpler
than c++ and all of the utility functions are already there in the library such as NumPy, scipy, and so on. At first, before all of the add-ons effect
to make the image look nicer, such as anti-aliasing, the code was running just fine at a good speed. However, after I've added anti-aliasing, 
the rendering process takes much longer time than expected, around 1 minute for a simple image with 10 samples. Realising that this would we 
bad when we moved to thousands of samples, I've decided to move to c++ as the author of the book recommended. Another reason that I've chosen c++ is
the fact that I can gain access to multiprocessing libraries such as OpenMP very easily. This will be used to speed up the rendering process even more.

### Theory / Straight from the book

As stated, most of the idea behind the implementation is followed directly from the book, this includes, camera, basic materials, hittable class abstraction, vector functions,
ray casting, sphere shape, and colour. Some have little tweaks in between but it is very minor.

The final output from the book is the following picture featuring all types of material,

![all_mat_types_512_samples](https://user-images.githubusercontent.com/83196403/158046149-41a3b59d-7f5a-45ba-83ff-221cc649c10a.png)

### Extension to light material and rectangular shape

#### Light material

After finishing the book and having a proper running code, I've decided also to try to extend from the idea discussed in the book regarding the material. I've tried
to implement light material by myself. There was a lot of confusion in the first place, but the idea, in the end, was very simple. What I implemented was just to 
return the attenuation of the light without scattering the ray (as this is the source of the light, it should not scatter in my opinion). This attenuation
would then get multiplied to the original colour it bounces from.

```c++
virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
     attenuation = albedo; // return light intensity
     return false;
}
```
For me, I interpret attenuation in this place to be the intensity of the light, so the value could be greater than one. This makes sense because,
the ray_color function is recursive which means that as the function returns the value of the light get decrease every step because it is multiplied to some
the factor which is the colour.

```c++
if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
     return attenuation * ray_color(scattered, world, depth - 1);
}
```

So if we have the light colour, when we instantiate the material, higher, the colour of the object that bounces off into the light source will be whiter / brighter.
And for rays that come from far away the less, the brightness will be.

First render of the custom light material,

![first_light_image_256_samples](https://user-images.githubusercontent.com/83196403/158045967-6defa67e-5594-4c1d-ac8f-bdb53016cfda.png)

This image has 256 samples per pixel and is using defocus blur, that is why even image in the light seems to be blurred. The light source here is
the white sphere on the very top of the picture. You can also see the shadow that depends on the light source unlike the first rendered image.
The first noticeable thing in this picture is the presence of black spots. 
After a lot of thinking, I've come to the conclusion that this occurred because too many light rays are randomly bouncing into the background which doesn't have any color (black). That is why the ball nearer to the light source have a fewer black spot because most of them are more likely to bounce into the light source. This could be solved with more sampling per pixel, but it will be cost more time to render. 

To work around this, I've realised that if the object is inside an enclosed space the light ray will bounce and share the light with another object more easily, as none of them will
go into the void. To test this, I've set up a simple scene where there is a light source on the left and also a hole, the rest of the sides are closed.

![light_room](https://user-images.githubusercontent.com/83196403/158046264-039153b1-7078-4445-9e0f-59b3e5272818.png)

Compared to the first rendered image with light source which is in an open space, this renders only take 128 samplings and is much clearer than the first image. As can be seen, the overall picture is brighter and much more illuminated than the first one. However, there's still a black spot because one of the sides is still open
into the void.

![light_box](https://user-images.githubusercontent.com/83196403/158046185-516604cc-741a-4178-8ec4-a3a12ecdb81e.png)

<p align="center"> <em> The picture with balls inside an enclosed space with light source on the left in distance. </em> </p>

Note that in this picture the light source is futher away and has less intensity than the one above this picutre, that is why it looks dimmer.
As you can see the black spots are lesser now when we enclose the object and light source in a closed space.

To finish this I've increase the intensity of the light but also open the back side, and render the picture again,

![open_back_room_with_light_256_samples](https://user-images.githubusercontent.com/83196403/158046568-9c39fdad-b9af-4723-a28c-1a09fc4bed7a.png)
<p align="center"> <em> 256 samples per pixel </em> </p>


![open_back_room_with_light_512_samples](https://user-images.githubusercontent.com/83196403/158046580-49a716be-995f-495d-9bb7-5d3ddfe6bf0c.png)
<p align="center"> <em> 512 samples per pixels </em> </p>

![open_back_room_with_light_768_samples](https://user-images.githubusercontent.com/83196403/158046585-3955f0a0-ebac-4910-b6e1-23de276167e4.png)
<p align="center"> <em> 768 samples per pixels </em> </p>

As you can see, the image is brighter and have less and less black spots as we add in more samples.

#### Rectangular Surface

For me, I personally feel that this much more challenging to implement than the light material, as it requries me not just to understand the concept but also to figure 
out the math behind it too. 

Rectangle are very simliar in nature to that of a plane but with restricted boundary. Therefore, I decided that it will be easy to to represent the 
rectangle as a whole plane as it will be easy to store then we can do boundary checking later. 

The equation of a plane is given by

```math
n \cdot (r - r_0) = 0
```

and line math $`a^2 + b^2 = c^2`$.

$$
n \cdot (r - r_0) = 0
$$

where n is the normal vector if the plane, and $r_0$ is the point on the plane. Which is equivalent to the follwing,

let $<a,b,c> = n, <x,y,z> = r, <x_0, y_0, z_0> = r_0$

$$
<a,b,c> \cdot (x - x_0, y - y_0, z - z_0) = 0
$$

Expanding this,

$$
ax + by + cz = ax_0 + by_0 + cz_0
$$

let $ax_0 + by_0 + cz_0 = d$

we get,

$$
ax + by + cz = d
$$

With the equation of a line is given by,

$$
r = r_0 + dt
$$

where d is a directional vector, or in another form,

$$
x = x_0 + at \\ 
y = y_0 + bt \\
z = z_0 + ct 
$$

To solve for line-plane intersection we can substitute the x, y, z in and solve for t, so,

$$
a(x_0 + at) + b(y_0 + bt) + c(z_0 + ct) = d
$$

simplifying this we get,

$$
t = \frac{d - (n \cdot r_0)}{n \cdot d}
$$


## NotesOn Reference Material

For most of the part, I've followed directly from the book called "Ray Tracing in One Weekend". With some vector utility functions 
being taken directly from the book in the c++ implementation version of my code. On some occasions, I would deviate from the book
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
