import struct
def readParameters():
	
	with open("input3",'rb') as fin:
		(m,) = struct.unpack("i",fin.read(4)) 
		(n,) = struct.unpack("i",fin.read(4))
		v = [[0]*(m+1) for i in range(n)]
		h = [[0]*(m) for i in range(n+1)]
		for i in range(n):
			for j in range(m+1):
				(v[i][j],) = struct.unpack("d",fin.read(8))
		for i in range(n+1):
			for j in range(m):
				(h[i][j],) = struct.unpack("d",fin.read(8))
	return m, n, v, h
				
def main():
    (m, n, v, h) = readParameters();
	
	# yours code
    return 0
	
if __name__ == "__main__":
	main()

