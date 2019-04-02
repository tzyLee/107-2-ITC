import numpy as np

arr = np.loadtxt("matrix_in.txt", skiprows=1)
a, b = np.split(arr, 2)

a = a.reshape((1000, 1000))
b = b.reshape((1000, 1000))
np.savetxt("validate_py.txt", a@b, fmt='%.6f')