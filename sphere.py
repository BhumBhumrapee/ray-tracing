from hittable import *
import math

class sphere(hittable):

    def __init__(self, cen: List, r: float):
        self.center = np.array(cen);
        self.radius = r;

    def __str__(self) -> str:
        return f"Sphere, center: {self.center}, radius: {self.radius}"
    
    def hit(self, r: ray, t_min: float, t_max: float, rec: hit_record) -> bool:
        oc = r.origin - self.center
        a = np.dot(r.dir, r.dir) #length sqaured
        half_b = np.dot(oc, r.dir)
        c = np.dot(oc, oc) - self.radius ** 2

        discriminant = half_b ** 2 - a * c
        if (discriminant < 0):
            return False
        
        sqrt_discriminant = math.sqrt(discriminant)
        root = (- half_b - sqrt_discriminant)/a
        if (root < t_min or t_max < root):
            root = (- half_b + sqrt_discriminant) / a
            if (root < t_min or t_max < root):
                return False
        
        rec.t = root
        rec.p = r.at(rec.t)
        outward_normal = (rec.p - self.center) / self.radius
        rec.set_face_normal(r, outward_normal)

        return True