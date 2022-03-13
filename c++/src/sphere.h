#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {

    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    
    vec3 center_to_origin = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(center_to_origin, r.direction());
    double c = center_to_origin.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) { // if there is no solution, we return immediately
        return false;
    }

    double sqrt_discriminant = sqrt(discriminant);
    double root = (-half_b - sqrt_discriminant) / a;

    if (root < t_min || t_max < root) { // if the first root lies outside the range, we check for the other one
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root) {
            return false; // if it is still outisde, just return
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif