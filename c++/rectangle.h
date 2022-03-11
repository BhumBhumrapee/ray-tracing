#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "hittable.h"
#include "vec3.h"

class rectangle : public hittable {

    public:
        rectangle() {}
        rectangle(point3 corner, point3 end_point_1, point3 end_point_2)
            : corner(corner), end_point_1(end_point_1), end_point_2(end_point_2) {
                vec3 vec_1 = end_point_1 - corner;
                vec3 vec_2 = end_point_2 - corner;
                normal = cross(vec_1, vec_2);
                d = dot(normal, corner);
            }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 corner;
        point3 end_point_1;
        point3 end_point_2;

    private:
        vec3 normal;
        double d;

};

bool rectangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return false;
}

#endif