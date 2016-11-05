#!/usr/bin/env python2.7
import os
import sys
import subprocess
import string

def runTest_nonideal(host_number):
    os.system("smpirun --cfg=smpi/running-power:4638671875.000 -np "+host_number +" -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul_compute 1600 >> output_nonideal.txt")
def runTest_ideal(host_number):
    os.system("smpirun --cfg=smpi/running-power:4638671875.000 -np "+host_number +" -hostfile ./hostfile_1600.txt -platform ./cluster_ideal.xml ./matmul_compute 1600 >> output_ideal.txt")

def runTest_comm(size):
    os.system("smpirun --cfg=smpi/running-power:4638671875.000 -np 16  -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul_comm "+str(size) +" >> output_comm.txt")

def runTest_compute(size):
    os.system("smpirun --cfg=smpi/running-power:4638671875.000 -np 16  -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul_compute "+str(size) +" >> output_compute.txt")

os.system("make clean")
os.system("make matmul_compute")
# Step 1
hosts=["1", "4", "16", "64", "100", "400"]
for host in hosts:
    for i in range(5):
        runTest_nonideal(host)
        runTest_ideal(host)

# Step 2
os.system("make clean")
os.system("make matmul_comm")
for n in range(100, 401, 10):
    for i in range(5):
        runTest_comm(8*n)
os.system("make matmul_compute")
for n in range(100, 401, 10):
    for i in range(5):
        runTest_compute(8*n)
        
