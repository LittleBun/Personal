CC=smpicc
CFLAGS= -O3

RUN=smpirun
NP=-np 5
HOSTFILE=-hostfile ./simple_cluster_hostfile.txt
PLATFORM=-platform ./simple_cluster.xml
NUMROW=1000

all: 1D_parallel_julia

basic: CFLAGS += -D BASIC
basic: exercise1

tiled: CFLAGS += -D TILED
tiled: exercise1

1D_parallel_julia: 1D_parallel_julia.o
	$(CC) 1D_parallel_julia.o -o 1D_parallel_julia

exercise1.o: 1D_parallel_julia.c
	$(CC) $(CFLAGS) $(PFLAGS) 1D_parallel_julia.c 

run:
	$(RUN)
clean:
	rm *.o
real_clean:
	rm exercise1
