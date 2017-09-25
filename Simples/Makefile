CC=gcc
CFLAGS= -O3
OMP=
LIB=
DEBUG=
PROF=

matmul_simples: matmul_simples.o
	$(CC) matmul_simples.o -o matmul_simples $(OMP) $(CFLAGS) $(LIB) $(PROF) $(DEBUG)

matmul_simples.o: matmul_simples.c
	$(CC) matmul_simples.c -c -o matmul_simples.o

clean:
	rm *.o matmul_simples
