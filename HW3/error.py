import numpy as np

mine = np.loadtxt('matrix_out.txt')
py = np.loadtxt('validate_py.txt')
val = np.loadtxt('validate_out.txt')

print('Error between mine and validate.cpp:', np.mean(np.abs(mine - val)))
print('Error between mine and validate.py:', np.mean(np.abs(mine - py)))
print('Error between validate.cpp and validate.py:', np.mean(np.abs(py - val)))