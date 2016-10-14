CC=smpicc
CFLAGS= -O3

RUN=smpirun
RUNFLAG=--cfg=smpi/bcast:mpich
NP=-np 50
HOSTFILE=-hostfile ./hostfile_50.txt
PLATFORM=-platform ./ring_50.xml
BINARYPLATFORM=-platform ./bintree_50.xml
CROSSPLATFORM=-platform ./cluster_crossbar_64.xml
BACKBONEPLATFORM=-platform ./cluster_backbone_64.xml
CHUNKSIZE=-c 1000000

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
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(BINARYPLATFORM) ./bcast_skeleton asynchronous_pipelined_bintree_bcast $(CHUNKSIZE)
run_default:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton default_bcast

run_asynch_pl_ring_bplf:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(BINARYPLATFORM) ./bcast_skeleton asynchronous_pipelined_ring_bcast $(CHUNKSIZE)
run_asynch_pl_bt_rplf:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(PLATFORM) ./bcast_skeleton asynchronous_pipelined_bintree_bcast $(CHUNKSIZE)

run_tree_crossbar:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(CROSSPLATFORM) ./bcast_skeleton asynchronous_pipelined_bintree_bcast $(CHUNKSIZE)
run_ring_crossbar:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(CROSSPLATFORM) ./bcast_skeleton asynchronous_pipelined_ring_bcast $(CHUNKSIZE)
run_default_crossbar:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(CROSSPLATFORM) ./bcast_skeleton default_bcast

run_tree_backbone:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(BACKBONEPLATFORM) ./bcast_skeleton asynchronous_pipelined_bintree_bcast $(CHUNKSIZE)
run_ring_backbone:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(BACKBONEPLATFORM) ./bcast_skeleton asynchronous_pipelined_ring_bcast $(CHUNKSIZE)
run_default_backbone:
	$(RUN) $(RUNFLAG) $(NP) $(HOSTFILE) $(BACKBONEPLATFORM) ./bcast_skeleton default_bcast

clean:
	rm bcast_skeleton
