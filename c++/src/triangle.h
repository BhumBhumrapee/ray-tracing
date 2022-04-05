#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {

    public:
        triangle() {}
        triangle(point3 ptn1, point3 ptn2, point3 ptn3, shared_ptr<material> m)
            : ptn1(ptn1), ptn2(ptn2), ptn3(ptn3), mat_ptr(m)  {
                vec3 vec_1 = ptn2 - ptn1;
                vec3 vec_2 = ptn3 - ptn1;
                normal = cross(vec_1, vec_2); // normal vector of a plane
                d = dot(normal, ptn1); // the constant d of equation of plane, ax + by + cz = d
            }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 ptn1;
        point3 ptn2;
        point3 ptn3;

    private:
        vec3 normal;
        double d;
        shared_ptr<material> mat_ptr;

};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double error_torelance = 0.0001;
    vec3 ray_direction = r.direction();
    bool has_solution = abs(dot(normal, ray_direction)) > error_torelance;
    if (!has_solution) { // if no sol, return immediately
        return false;
    }
    double t = (d - dot(normal, r.origin())) / dot(normal, ray_direction);

    if (t < t_min || t > t_max) { // or if the sol is greater than the boundary we also return
        return false;
    }

    point3 intersection = r.at(t);

    vec3 corner_to_inter_1 = unit_vector(intersection - ptn1);
    vec3 corner_to_inter_2 = unit_vector(intersection - ptn2);
    vec3 corner_to_inter_3 = unit_vector(intersection - ptn3);
    
    vec3 vec_1 = unit_vector(ptn2 - ptn1);
    vec3 vec_2 = unit_vector(ptn3 - ptn2);
    vec3 vec_3 = unit_vector(ptn1 - ptn3);

    float angle_1 = acos(dot(vec_1, -vec_3) / (vec_1.length() * vec_3.length()));
    float angle_2 = acos(dot(vec_2, -vec_1) / (vec_2.length() * vec_1.length()));
    float angle_3 = acos(dot(vec_3, -vec_2) / (vec_3.length() * vec_2.length()));

    bool con1 = acos(dot(vec_1, corner_to_inter_1) / (vec_1.length() * corner_to_inter_1.length())) <= angle_1;
    bool con2 = acos(dot(vec_2, corner_to_inter_2) / (vec_2.length() * corner_to_inter_2.length())) <= angle_2;
    bool con3 = acos(dot(vec_3, corner_to_inter_3) / (vec_3.length() * corner_to_inter_3.length())) <= angle_3;

    if (con1 && con2 && con3) { 
        rec.t = t;
        rec.p = r.at(t);
        rec.set_face_normal(r, normal);
        rec.mat_ptr = mat_ptr;
        return true; 
    }

    return false;
}

#endif