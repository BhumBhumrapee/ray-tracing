import numpy as np

class ray:

    def __init__(self, origin, direction):
        self.origin = origin.copy();
        self.dir = direction.copy();

    def at(self, t):
        return self.origin + np.array(list(map(lambda x: t * x, self.dir)));
    
    def __str__(self) -> str:
        return f"ori: {self.origin}, dir: {self.dir}"