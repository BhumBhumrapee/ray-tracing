import numpy as np
from utils import *

p = np.array([-0.13342156, 0.39728934, -0.72730993])
origin = np.array([0, 0, -1])
normal = np.array([-0.26684311, 0.79457867, 0.54538013])
target = np.array([-0.32718628, 0.96469314, 0.36559692])

print(target - p)