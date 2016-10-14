#!/usr/bin/env python2.7
import os
import sys
import subprocess
import string

def runTest(implement_name):
   os.system("mpirun ./bcast_skeleton " + implement_name +  " -c 1000000 >> output.txt")

os.system("mpicc -O3 bcast_skeleton.c -o bcast_skeleton")
#hosts=["20", "35", "50"]
#chunk_size=["100000", "500000", "1000000", "5000000", "10000000", "50000000", "100000000"]
implement_name=["default_bcast", "naive_bcast", "asynchronous_pipelined_ring_bcast", "asynchronous_pipelined_bintree_bcast"]
#runTest("50", "100000", "pipelined_ring_bcast")
for name in implement_name:
    for trial in range(10):
        runTest(name)
