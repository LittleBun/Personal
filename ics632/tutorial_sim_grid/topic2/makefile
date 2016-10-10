CC=smpicc
CFLAGS= -O3

RUN=smpirun
RUNFLAG=--cfg=smpi/bcast:mpich
NP=-np 50
HOSTFILE=-hostfile ./hostfile_50.txt
PLATFORM=-platform ./ring_50.xml
CHUNKSIZE=-c 500000

all: bcast_skeleton

bcast_skeleton: bcast_skeleton.c
	$(CC) $(CFLAGS) bcast_skeleton.c -o bcast_skeleton 

run_naive:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton naive_bcast
run_ring:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton ring_bcast
run_pl_ring:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton pipelined_ring_bcast $(CHUNKSIZE)
run_asynch_pl_ring:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton asynchronous_pipelined_ring_bcast $(CHUNKSIZE)
run_asynch_pl_bt:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton asynchronous_pipelined_bintree_bcast
run_default:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton default_bcast

clean:
	rm bcast_skeleton
