import numpy
import sys


def createMatrix():
	if(len(sys.argv) == 4):
		m = int(sys.argv[1])
		n = int(sys.argv[2])
		name = sys.argv[3]

		file = open(name+ '.txt', 'w')

		mat = numpy.random.rand(m, n)
		mat *= 20
	
		for i in range(m):
			for j in range(n):
				print (str(mat[i][j]) + ' ')
			print('\n')
		

createMatrix()
