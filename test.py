import numpy as np

for t in np.arange(0, 20, 0.2):
    print(1-np.exp(-t/5))
