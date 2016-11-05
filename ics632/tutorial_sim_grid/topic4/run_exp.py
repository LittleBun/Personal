#!/usr/bin/env python2.7
import os
import sys
import subprocess
import string

def runTest_nonideal(host_number):
    os.system("smpirun --cfg=smpi/running-power:7812500000 -np "+host_number +" -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul 1600 >> output_nonideal.txt")

def runTest_ideal(host_number):
    os.system("smpirun --cfg=smpi/running-power:7812500000 -np "+host_number +" -hostfile ./hostfile_1600.txt -platform ./cluster_ideal.xml ./matmul 1600 >> output_ideal.txt")

def runTest_comm(size):
    os.system("smpirun --cfg=smpi/running-power:7812500000 -np 16  -hostfile ./hostfile_1600.txt -platform ./cluster_nonideal.xml ./matmul "+str(size) +" >> output_comm.txt")

os.system("make matmul")
os.system("rm output_idea.txt")
os.system("rm output_nonidea.txt")
os.system("rm output_comm.txt")

# Step 1
hosts=["1", "4", "16", "64", "100", "400"]
for host in hosts:
    for i in range(5):
        runTest_nonideal(host)
        runTest_ideal(host)

# Step 2
for n in range(100, 401, 10):
    for i in range(5):
        runTest_comm(8*n)
