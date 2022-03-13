#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "ray.h"
#include "hittable.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {

    public: 
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector_in_unit_sphere();

            if (scatter_direction.near_zero()) { // if the randomed unit vector is near to to opposite of the normal vector, this could lead to zero vec, so we return normal instead
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_vector_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};

class dielectric: public material {

    public:
        
        dielectric(double ir): index_of_refraction(ir) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1,1,1);
            double refraction_ratio = rec.front_face ? (1.0 / index_of_refraction) : index_of_refraction;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1; // total internal reflection
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, rec.normal); // if cannot refract or the reflectance is more than a certain amount, just let it reflect.
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double index_of_refraction;

    private:
        static double reflectance(double cosine, double ref_idx) {
            // an approximation by Schlick for glass reflection
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine),5);
        }
};


class light : public material {

    public:

        light(const color& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = albedo;
            return false;
        }

    public:
        color albedo;
};



#endif