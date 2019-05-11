import heap as h

heap = h.Heap()
opNum = 0
heapNum = 0
numCount = 0
opCount = 0
key = []
element = []
index = 0
with open("input.txt") as infile:
	for line in infile:
		if opNum == 0:
			opNum = int(line)
		elif heapNum == 0:
			heapNum = int(line)
		elif numCount < heapNum:
			l = line.split()
			key.append( int( l[0] ) )
			element.append( l[1] )
			numCount += 1
		elif opCount < opNum:
			if int(line) == 1:
				heap.push(key[index], element[index])
				index += 1
			else:
				heap.pop()
			opCount += 1			
heap.printArray()
heap.printByPopping()
