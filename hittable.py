from abc import ABC, abstractmethod

from numpy import save
from ray import *
from typing import List

class hit_record:
    
    def __init__(self, p: List = [1, 1, 1], normal: List = [1, 1, 1], t: float = 0, front_face: bool = False) -> None:
        self.p = np.array(p)
        self.normal = np.array(normal)
        self.t = t
        self.front_face = front_face

    def set_face_normal(self, r: ray, outward_normal):
        self.front_face = np.dot(r.dir, outward_normal) < 0
        self.normal = outward_normal if self.front_face else np.dot(outward_normal, -1)

    def __str__(self) -> str:
        return f"p: {self.p}, normal: {self.normal}, t: {self.t}, front_face: {self.front_face}"

    def assign_properties_to(self, other):
        self.p = other.p
        self.normal = other.normal
        self.t = other.t
        self.front_face = other.front_face

class hittable(ABC):
    
    @abstractmethod

    def hit(self, r: ray, t_min: float, t_max: float, rec: hit_record) -> bool:
        pass


class hittable_list(hittable):

    def __init__(self, *args):
        self.objects: List[hittable] = []
        if (len(args) > 0):
            self.objects.append(args[0])
    
    def clear(self):
        self.objects.clear()
    
    def add(self, object):
        self.objects.append(object)
    
    def hit(self, r: ray, t_min: float, t_max: float, rec: hit_record) -> bool:
        temp_rec: hit_record = hit_record()
        hit_anything = False;
        closet_so_far = t_max;

        for object in self.objects:
            if (object.hit(r, t_min, closet_so_far, temp_rec)):
                hit_anything = True
                closet_so_far = temp_rec.t;
                rec.assign_properties_to(temp_rec)
    

        return hit_anything