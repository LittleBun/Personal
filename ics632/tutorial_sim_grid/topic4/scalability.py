#!/usr/bin/env python2.7
import os
import sys
import subprocess
import string

def runTest_nonideal(size):
    os.system("smpirun --cfg=smpi/running-power:4638671875.000 -np 100 -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul "+ str(size)+" >> output_scale.txt")

os.system("make matmul")
# Step 1
for size in range(1000, 16001, 1000):
    for i in range(5):
        print i
        runTest_nonideal(size)
