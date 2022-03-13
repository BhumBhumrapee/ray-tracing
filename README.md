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
the fact that I can gain access to multiprocessing libraries such as OpenMP very easily. This will be used to speed up the rendering process even more. I've splitted 
my learning into two part, which is the part where i follow the book, and the part where I go out and try things on my own. Let's start with the theory from the book.

### Theory / Straight from the book

As stated, most of the idea behind the implementation is followed directly from the book, this includes, camera, basic materials, hittable class abstraction, vector functions,
ray casting, sphere shape, and colour. Some have little tweaks in between but it is very minor. I've learned a lot through reading the whole book although I have already learned most of the math and physics concepts discussed inside from physics, linear algebra and calculus class, the book is the one that connects and bring it all to life. 

The final output from the book is the following picture featuring all types of material,

![all_mat_types_512_samples](https://user-images.githubusercontent.com/83196403/158046149-41a3b59d-7f5a-45ba-83ff-221cc649c10a.png)

### Extension to light material and rectangular shape

Although the project was very enjoyable, this is where the fun part begins. Where I go try and struggle to implement things not discussed in the book by my own. Let's start with the light material, where I will walkthough my thinking process.

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
the factor which is the colour. Therefore, the higher the value the further the light travel (stay alive) and the brighter it is.

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

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/latex/eq_of_plane.PNG"> </p>

First off, I've decided to represent the plane by three points called, corner, end_point_1, and end_point_2. From this we can deduce two vectors which will be
on the plane by,

```c++
vec3 vec_1 = end_point_1 - corner;
vec3 vec_2 = end_point_2 - corner;
```

From this we can deduce the last point by vector addition,

```c++
end_point_3 = vec_1 + vec_2 + corner;
```

To get the normal vector of a plane, we can cross the two vectors

```c++
normal = cross(vec_1, vec_2); // normal vector of a plane
```

and d is just,

```c++
d = dot(normal, cn); // the constant d of equation of plane, ax + by + cz = d
```

Now to solve for the intersection,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/latex/intersection.PNG"> </p>

Now that we have the t we can easily solve for the point of intersection very easily,

```c++
point3 intersection = r.at(t);
```

Note that there might not be a solution, e.g the ray is parallel to the plane. This happens when the demoniminator is zero which means that
the ray is perpendicular to the normal of the plane which means it is parallel to the plane itself, so we can check that first to avoid dividing by
zero error.

```c++
double error_torelance = 0.0001;
vec3 ray_direction = r.direction();
bool has_solution = abs(dot(normal, ray_direction)) > error_torelance;
if (!has_solution) { // if no sol, return immediately
    return false;
}       
```

Now that we have the intersection, here comes the hard part. How do we check if the point lies within the rectangle. The first thing that comes to my 
mind if projection. We project the plane along with the intersection point onto xy, xz, or yz plane and the do the boundary checking on the projected image.
However, I feel that this is clunky and that there are a alot of case to cover. For example, if the plane is already aligned with the yz plane or xz plane, projecting onto
the xy plane will result in a plane with zero dimension (a line), and therefore we cannot check for height. To work around this we will have to check first if 
the plane is aligning to which plane and then project onto the other. I feel like that this is bug prone so I drop the idea.

After a lot of thinking I sense that this has something to do with dot product and the angles between vector the corner vectors and the vector to the intersection point, however, the idea is not solid / clear to me at the time yet. I drew out pictures and then I suddenly realise how to do this (although I didn't prove any of this, so there might be some case where I missed and I could be wrong). 

Here's the setup,

Take the intersection point minus the corner of the rectangle to get a vector, call this corner_to_inter_1. Then do the same for the opposite corner and call it
corner_to_inter_2. Now we have two vectors that points from each corner to the intersection point.

For each corner that the corner_to_inter vector points out from, find the neighbhor point, take those point and minus it with the corner points. So,
in total we will have 6 total vectors,

```c++
vec3 corner_to_inter_1 = unit_vector(intersection - corner);
vec3 corner_to_inter_2 = unit_vector(intersection - end_point_3);

vec3 vec_1 = unit_vector(end_point_1 - corner);
vec3 vec_2 = unit_vector(end_point_2 - corner);
vec3 vec_3 = unit_vector(end_point_1 - end_point_3);
vec3 vec_4 = unit_vector(end_point_2 - end_point_3);
```

to illustrate this I've made a picture,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/latex/illus/2x/vectors.png"> </p>

Using the property of dot project, if the angle between the vectors are more than 90 degress, then it will be negative we can check if the point lies within the
rectangle.

If the dot product between te corner to each of its' neighnor vector is less than zero, we know that the point must lies outside the box for sure. For example,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/latex/illus/2x/vectors_out.png"> </p>

In the following picture the dot product between u and its' neighbhor vector w will be less than zero. Or on another side,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/latex/illus/2x/vectors_out_2.png"> </p>

The dot product between v1 and u1 will be less than zero while others will all be positive.

Therefore if the point lies inside the rectangle, the dot product of all four vectors to its correspoding conrner to intersection vector must be positive. 
In code,

```c++
bool con1 = dot(vec_1, corner_to_inter_1) > 0;
bool con2 = dot(vec_2, corner_to_inter_1) > 0;
bool con3 = dot(vec_3, corner_to_inter_2) > 0;
bool con4 = dot(vec_4, corner_to_inter_2) > 0;
if (con1 && con2 && con3 && con4) { // check if the point lies within the rectangle
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
    return true; 
}
return false;
```

This is much better than the projection method, because it doesn't require checking all the case then projecting then do checking again.
Including this in the scene too we get,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/c%2B%2B/images/open_light_rec_128_samples.png"> </p>
<p align="center"> <em> 128 samples per pixel, open space</em> </p>

I used the rectangle for light sources as it looks cooler than a sphere. Now with more samplings,

<p align="center"> <img src="https://github.com/BhumBhumrapee/ray-tracing/blob/master/c%2B%2B/images/open_light_rec_512_samples.png"> </p>
<p align="center"> <em> 512 samples per pixel, open space</em> </p>

This concludes my project.

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
