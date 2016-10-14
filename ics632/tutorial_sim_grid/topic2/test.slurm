#!/bin/bash
#SBATCH -n 50 #n of tasks
#SBATCH -c 1 #n of cores
#SBATCH -N 3 #n of nodes
#SBATCH -t 200 #runtime in minutes
#SBATCH -o test.out #std out goes o this file
#SBATCH -e test.err #std err goes to this file
#SBATCH --mail-type ALL
#SBATCH --mail-user hualiang@hawaii.edu
#SBATCH --account=ics_632
#SBATCH -p ics632.q

#module load intel/ics
#module load tools/simgrid/3.13_stable
module load intel/impi
python test.py
#make run_default1
#perf stat ./fast_p 20 2
#python run_all.py
#ls
