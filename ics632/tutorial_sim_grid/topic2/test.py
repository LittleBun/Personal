#!/usr/bin/env python2.7
import os
import sys
import subprocess
import string

def runTest(host_number, chunk_size, implement_name):
   os.system("smpirun --cfg=smpi/bcast:mpich -np "+ host_number+" -platform ring_"+host_number+".xml -hostfile hostfile_"+host_number+".txt ./bcast_skeleton " + implement_name + " -c " + chunk_size+" >> output.txt")

os.system("smpicc -O3 bcast_skeleton.c -o bcast_skeleton")
hosts=["20", "35", "50"]
chunk_size=["100000", "500000", "1000000", "5000000", "10000000", "50000000", "100000000"]
implement_name=["pipelined_ring_bcast", "asynchronous_pipelined_ring_bcast"]
#runTest("50", "100000", "pipelined_ring_bcast")
for host in hosts:
   for size in chunk_size:
      runTest(host, size, "pipelined_ring_bcast")
for size in chunk_size:
   runTest("50", size, "asynchronous_pipelined_ring_bcast")
