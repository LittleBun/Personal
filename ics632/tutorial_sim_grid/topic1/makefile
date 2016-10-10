CC=smpicc
CFLAGS= -O3

RUN=smpirun
NP=-np 9
HOSTFILE=-hostfile ./simple_cluster_hostfile.txt
PLATFORM=-platform ./simple_cluster.xml
NUMROW=300

all: 1D_parallel_julia 2D_parallel_julia

1D_parallel_julia: 1D_parallel_julia.c
	$(CC) $(CFLAGS) 1D_parallel_julia.c -o 1D_parallel_julia 

2D_parallel_julia: 2D_parallel_julia.c
	$(CC) $(CFLAGS) 2D_parallel_julia.c -o 2D_parallel_julia 

run_1D:
	$(RUN) $(NP) $(HOSTFILE) $(PLATFORM) ./1D_parallel_julia $(NUMROW)
run_2D:
	$(RUN) $(NP) $(HOSTFILE) $(PLATFORM) ./2D_parallel_julia $(NUMROW)

clean:
	rm *.o
real_clean:
	rm 1D_parallel_julia 2D_parallel_julia
clean_data:
	rm julia.bmp
