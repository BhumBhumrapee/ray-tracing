#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "hittable.h"
#include "vec3.h"

class rectangle : public hittable {

    public:
        rectangle() {}
        rectangle(point3 cn, point3 end1, point3 end2, shared_ptr<material> m)
            : corner(cn), end_point_1(end1), end_point_2(end2), mat_ptr(m)  {
                vec3 vec_1 = end1 - cn;
                vec3 vec_2 = end2 - cn;
                end_point_3 = vec_1 + vec_2 + corner;
                normal = cross(vec_1, vec_2);
                d = dot(normal, cn);
                std::cerr << "normal: " << normal << ", vec1: " << vec_1 << ", vec2: " << vec_2 << ", corner: " 
                << corner << ", end1: " << end_point_1 << ", end2: " << end_point_2 << ", end3: " << end_point_3 << "\n";
            }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 corner; 
        point3 end_point_1;
        point3 end_point_2;
        point3 end_point_3;

    private:
        vec3 normal;
        double d;
        shared_ptr<material> mat_ptr;

};

bool rectangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double error_torelance = 0.0001;
    vec3 ray_direction = r.direction();
    bool has_solution = abs(dot(normal, ray_direction)) > error_torelance;
    if (!has_solution) {
        return false;
    }
    double t = (d - dot(normal, r.origin())) / dot(normal, ray_direction);

    if (t < t_min || t > t_max) {
        return false;
    }

    point3 intersection = r.at(t);

    vec3 corner_to_inter_1 = unit_vector(intersection - corner);
    vec3 corner_to_inter_2 = unit_vector(intersection - end_point_3);
    
    vec3 vec_1 = unit_vector(end_point_1 - corner);
    vec3 vec_2 = unit_vector(end_point_2 - corner);
    vec3 vec_3 = unit_vector(end_point_1 - end_point_3);
    vec3 vec_4 = unit_vector(end_point_2 - end_point_3);

    bool con1 = dot(vec_1, corner_to_inter_1) > 0;
    bool con2 = dot(vec_2, corner_to_inter_1) > 0;
    bool con3 = dot(vec_3, corner_to_inter_2) > 0;
    bool con4 = dot(vec_4, corner_to_inter_2) > 0;

    if (con1 && con2 && con3 && con4) {
        rec.t = t;
        rec.p = r.at(t);
        rec.set_face_normal(r, normal);
        rec.mat_ptr = mat_ptr;
        return true; 
    }

    return false;
}

#endif