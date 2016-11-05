CC=smpicc
CFLAGS= -Ofast
RUN_POW= --cfg=smpi/running-power:4638671875.000

RUN=smpirun
NP=-np 4
HOSTFILE=-hostfile ./hostfile_1600.txt
PLATFORM=-platform ./cluster_1600.xml
N=2000

all: matmul matmul_compute matmul_comm

# real compute without wallclock
matmul: matmul.c
	$(CC) $(CFLAGS) matmul.c -o matmul

# real compute with wallclock
matmul_compute: matmul_real.c
	$(CC) $(CFLAGS) matmul_real.c -DCP -o matmul_compute
matmul_comm: matmul_real.c
	$(CC) $(CFLAGS) matmul_real.c -DCM -o matmul_comm

# simulation
run: matmul
	$(RUN) $(RUN_POW) $(NP) $(HOSTFILE) $(PLATFORM) ./matmul $(N)

run_compute: matmul_compute
	$(RUN) $(RUN_POW) $(NP) $(HOSTFILE) $(PLATFORM) ./matmul_compute $(N)
run_comm: matmul_comm
	$(RUN) $(RUN_POW) $(NP) $(HOSTFILE) $(PLATFORM) ./matmul_comm $(N)

clean:
	rm matmul_init matmul matmul_compute matmul_comm
