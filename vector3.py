import numpy as np
from scipy import linalg

class vec3:
    def __init__(self, i = 0, j = 0, k = 0):
        self.pts = np.array([i, j, k])

    def __iadd__(self, o):
        self.pts = self.pts + o.pts
        return self.pts

    def __imul__(self, t):
        self.pts *= t
        return self.pts

    def __idiv__(self, t):
        self.pts *= 1/t
        return self.pts
    
    def __add__(self, o):
        return self.pts + o.pts

    def __sub__(self, o):
        return self.pts - o.pts


    def length(self):
        return linalg.norm(self.pts)

    def length_squared(self):
        return np.dot(self.pts, self.pts)

    def x(self):
        return self.pts[0]
    
    def y(self):
        return self.pts[1]

    def z(self):
        return self.pts[2]

    def __str__(self) -> str:
        return f"{self.pts[0]} {self.pts[1]} {self.pts[2]}"

if __name__ == "__main__":
    a = vec3(4,5,6)
    b = vec3(1,2,3)
    print(a.length_squared())
    a += b
    print(a)
    print(a - b)