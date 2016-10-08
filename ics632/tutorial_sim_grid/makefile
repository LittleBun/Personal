sequential_julia: sequential_julia.c
   smpicc sequential_julia.c -o sequential_julia -D p1

run: sequential_julia
   smpirun -platform ./simple_cluster.xml -np 4 -hostfile ./simple_cluster_hostfile.txt ./sequential_julia 100
