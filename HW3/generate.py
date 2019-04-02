import numpy as np

size = 3
a = np.random.randn(size, size)
b = np.random.randn(size, size)
fmt = '%.6f'

with open('test_in.txt', 'w') as f:
    f.write(str(size))
    f.write('\n')

with open('test_in.txt', 'ba') as f:
    np.savetxt(f, a, fmt=fmt)
    f.write(b'\n')
    np.savetxt(f, b, fmt=fmt)

np.savetxt('test_out.txt', a@b, fmt=fmt)