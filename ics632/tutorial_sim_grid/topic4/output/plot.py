#!/usr/bin/env python2.7
import matplotlib
import re
import string
matplotlib.use('Agg')
import matplotlib.pyplot as pyplot

output_nonideal = "output_nonideal.txt"
output_ideal = "output_ideal.txt"
output_comm = "output_comm.txt"
output_compute = "output_compute.txt"
output_scale = "output_scale.txt"

compute_plot = "./plot_compute.pdf"
comm_plot = "./plot_comm.pdf"
scale_plot = "./plot_scale.pdf"

def parseScaleOutput(filename):
    size = []
    time = []
    trial = 5
    count = 0
    sumtime = 0.0
    with open(filename) as f:
        lines = f.readlines()
    for line in lines:
        if (line.startswith('N')):
            count += 1
            line = re.sub('[,|=]', '', line)
            tokens = string.capwords(line).split(" ")
            sumtime += float(tokens[6])
        if count == trial:
            time.append(sumtime/trial)
            size.append(int(tokens[1]))
            count = 0
            sumtime = 0.0
    pyplot.figure(3)
    pyplot.plot(size,time,'r-',label='compute time',linewidth=2.0)
    pyplot.legend()
    pyplot.xlabel("size")
    pyplot.ylabel("time")
    pyplot.savefig(scale_plot,bbox_inchex='tight')
    print "Figure saved to file '"+scale_plot+"'"

def parseCommOutput(filename1, filename2):
    size = []
    time = []
    trial = 5
    count = 0
    sumtime = 0.0
    with open(filename1) as f:
        lines = f.readlines()
    for line in lines:
        if (line.startswith('N')):
            count += 1
            line = re.sub('[,|=]', '', line)
            tokens = string.capwords(line).split(" ")
            sumtime += float(tokens[6])
        if count == trial:
            time.append(sumtime/trial)
            size.append(int(tokens[1]))
            count = 0
            sumtime = 0.0
    pyplot.figure(2)
    pyplot.plot(size,time,'r-',label='communicate time',linewidth=2.0)

    size = []
    time = []
    trial = 5
    count = 0
    sumtime = 0.0
    with open(filename2) as f:
        lines = f.readlines()
    for line in lines:
        if (line.startswith('N')):
            count += 1
            line = re.sub('[,|=]', '', line)
            tokens = string.capwords(line).split(" ")
            sumtime += float(tokens[6])
        if count == trial:
            time.append(sumtime/trial)
            size.append(int(tokens[1]))
            count = 0
            sumtime = 0.0
    pyplot.figure(2)
    pyplot.plot(size,time,'b-',label='compute time',linewidth=2.0)

    pyplot.legend()
    pyplot.xlabel("size")
    pyplot.ylabel("time")
    pyplot.savefig(comm_plot,bbox_inchex='tight')
    print "Figure saved to file '"+comm_plot+"'"

def parseComputeOutput(filename1, filename2):
    procs = []
    time = []
    trial = 5
    count = 0
    sumtime = 0.0
    with open(filename1) as f:
        lines = f.readlines()
    for line in lines:
        if (line.startswith('N')):
            count += 1
            line = re.sub('[,|=]', '', line)
            tokens = string.capwords(line).split(" ")
            sumtime += float(tokens[6])
        if count == trial:
            time.append(sumtime*1000/trial)
            procs.append(int(tokens[3]))
            count = 0
            sumtime = 0.0
    pyplot.figure(1)
    pyplot.plot(procs,[max(time)/float(x) for x in time],'r-',label='nonideal speedup',linewidth=2.0)

    procs = []
    time = []
    trial = 5
    count = 0
    sumtime = 0.0
    with open(filename2) as f:
        lines = f.readlines()
    for line in lines:
        if (line.startswith('N')):
            count += 1
            line = re.sub('[,|=]', '', line)
            tokens = string.capwords(line).split(" ")
            sumtime += float(tokens[6])
        if count == trial:
            print "LINE=", line, "SUMTIME=", sumtime
            time.append(sumtime*1000/trial)
            procs.append(int(tokens[3]))
            count = 0
            sumtime = 0.0
    pyplot.figure(1)
    pyplot.plot(procs,[max(time)/float(x) for x in time],'b-',label='ideal speedup',linewidth=2.0)

    pyplot.legend()
    pyplot.xlabel("processes size")
    pyplot.ylabel("speedup")
    pyplot.savefig(compute_plot,bbox_inchex='tight')
    print "Figure saved to file '"+compute_plot+"'"


#parseComputeOutput(output_nonideal, output_ideal)
#parseCommOutput(output_comm, output_compute)
parseScaleOutput(output_scale)
