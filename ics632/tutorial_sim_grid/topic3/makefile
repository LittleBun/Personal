CC=smpicc
CFLAGS= -O3

RUN=smpirun
NP=-np 4
HOSTFILE=-hostfile ./hostfile_100.txt
PLATFORM=-platform ./cluster_crossbar_64.xml
N=1600

all: matmul_init matmul

matmul_init: matmul_init.c
	$(CC) $(CFLAGS) matmul_init.c -o matmul_init 

matmul: matmul.c
	$(CC) $(CFLAGS) matmul.c -o matmul

run_init:
	$(RUN) $(NP) $(HOSTFILE) $(PLATFORM) ./matmul_init $(N)
run: all
	$(RUN) $(NP) $(HOSTFILE) $(PLATFORM) ./matmul $(N)

clean:
	rm matmul_init matmul
