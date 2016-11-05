#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

//#define DEBUG
#define E 1.1
#define FLOP_CALIBRATION_FACTOR 0.572   // 0.04640435 seconds

///////////////////////////////////////////////////////
//// program_abort() and print_usage() functions //////
///////////////////////////////////////////////////////

static void program_abort(char *exec_name, char *message);
static void print_usage();
void mat_mul(double *A, double *B, double *C, int size);

// Abort, printing the usage information only if the
// first argument is non-NULL (and hopefully set to argv[0]), and
// printing the second argument regardless.
static void program_abort(char *exec_name, char *message) {
   int my_rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   if (my_rank == 0) {
      if (message) {
         fprintf(stderr,"%s",message);
      }
      if (exec_name) {
         print_usage(exec_name);
      }
   }
   MPI_Abort(MPI_COMM_WORLD, 1);
   exit(1);
}

// Print the usage information
static void print_usage(char *exec_name) {
   int my_rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

   if (my_rank == 0) {
      fprintf(stderr,"Usage: smpirun -np <num processes>\n");
      fprintf(stderr,"              -platform <XML platform file> -hostfile <host file>\n");
      fprintf(stderr,"              %s <matmul_init> < matrix size>\n",exec_name);
      fprintf(stderr,"MPIRUN arguments:\n");
      fprintf(stderr,"\t<num processes>: number of MPI processes\n");
      fprintf(stderr,"\t<XML platform file>: a Simgrid platform description file\n");
      fprintf(stderr,"\t<host file>: MPI host file with host names from the platform file\n");
      fprintf(stderr,"PROGRAM arguments:\n");
      fprintf(stderr,"\t[<matrix size>]: matrix size\n");
      fprintf(stderr,"\n");
   }
   return;
}

void mat_mul(double *A, double *B, double *C, int size) {
   int i, j, k;
   for (k=0; k<size; k++) {
      for (i=0; i<size; i++) {
         for (j=0; j<size; j++) {
            C[i*size + j] += A[i*size + k] * B[k*size + j];
            //*(C + size*i + j) += (*(A + size*i + k)) * (*(B + size*k + j));
         }
      }
   }
}

void print_mat(double *A, int size) {
   int i, j;
   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         printf("%.1f ", A[i*size+j]);
      }
      printf("\n");
   }
}

///////////////////////////
////// Main function //////
///////////////////////////

int main(int argc, char *argv[])
{
   int i,j;
   int size;

   MPI_Init(&argc, &argv);

   // Bcast implementation name
   if (argc < 2) {
      program_abort(argv[0],"Missing <matrix size> argument\n");
   } else {
      size = atoi(argv[1]);
   }

   // Determine rank and number of processes
   int num_procs;
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
   int sqrt_num_procs = sqrt(num_procs);

   // Check that the implementation name is valid
   char message[256];
   if (sqrt_num_procs * sqrt_num_procs != num_procs) {
      sprintf(message, "Invalid number of processors, use a perfect square number.\n");
      program_abort(NULL,message);
   } else if (size % sqrt_num_procs != 0) {
      sprintf(message, "Invalid matrix dimention, use value that dividable by %d\n",sqrt_num_procs);
      program_abort(NULL,message);
   }

   // Allocate matrix buffer
   int checksum;
   int local_size= size/sqrt_num_procs;
   double *A;
   double *B;
   double *C;
   double *bufferA;
   double *bufferB;

   
   /*
   if ((A = malloc(sizeof(double) * local_size*local_size)) == NULL ||
         (B = malloc(sizeof(double) * local_size*local_size)) == NULL ||
         (C = calloc(local_size*local_size, sizeof(double))) == NULL ||
         (bufferA = malloc(sizeof(double) * local_size*local_size)) == NULL ||
         (bufferB = malloc(sizeof(double) * local_size*local_size)) == NULL ) {
      program_abort(argv[0],"Out of memory!");
   }
   */
   if ((A = SMPI_SHARED_MALLOC(sizeof(double) * local_size*local_size)) == NULL ||
         (B = SMPI_SHARED_MALLOC(sizeof(double) * local_size*local_size)) == NULL ||
         (C = SMPI_SHARED_MALLOC(sizeof(double) * local_size*local_size)) == NULL ||
         (bufferA = SMPI_SHARED_MALLOC(sizeof(double) * local_size*local_size)) == NULL ||
         (bufferB = SMPI_SHARED_MALLOC(sizeof(double) * local_size*local_size)) == NULL ) {
      program_abort(argv[0],"Out of memory!");
   }
   // Create cartesian virtual topology, get rank, coordinates, neighbor ranks
   int dims[2] = {sqrt_num_procs, sqrt_num_procs};
   int periods[2] = {0, 0};
   int reorder = 0;
   int coords[2];
   MPI_Comm cartcomm;

   MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm);
   MPI_Comm_rank(cartcomm, &rank);
   MPI_Cart_coords(cartcomm, rank, 2, coords);

   /*
      printf("rank= %d coords= %d %d neighbors(u, d, l, r)= %d %d %d %d\n",
      rank, coords[0], coords[1], nbrs[UP], brs[DOWN], nbrs[LEFT], nbrs[RIGHT]);
   */

   int row_comm_color = rank/sqrt_num_procs;   // color for each row communicator
   int col_comm_color = rank%sqrt_num_procs;   // color for each col communicator

   MPI_Comm row_comm;
   MPI_Comm_split(cartcomm, row_comm_color, rank, &row_comm);
   int row_comm_rank, row_comm_size;
   MPI_Comm_rank(row_comm, &row_comm_rank);
   MPI_Comm_size(row_comm, &row_comm_size);

   MPI_Comm col_comm;
   MPI_Comm_split(cartcomm, col_comm_color, rank, &col_comm);
   int col_comm_rank, col_comm_size;
   MPI_Comm_rank(col_comm, &col_comm_rank);
   MPI_Comm_size(col_comm, &col_comm_size);
   /*
   printf("WORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d  \t COL RANK/SIZE: %d/%d\n",
            rank, num_procs, row_comm_rank, row_comm_size, col_comm_rank, col_comm_size);
   */

   // Init matrix
   int row_begin, row_end;
   int col_begin, col_end;
   row_begin = coords[0] * local_size;
   row_end = row_begin + local_size - 1;
   col_begin = coords[1] * local_size;
   col_end = col_begin + local_size - 1;
   /*
   printf("rank=%d, row from %d to %d, col from %d to %d\n", 
                rank, row_begin, row_end, col_begin, col_end);
   */
   for (i = row_begin; i <= row_end; i++) {
      for (j = col_begin; j <= col_end; j++) {
         *(A+(i-row_begin)*local_size+(j-col_begin)) = i;
         *(B+(i-row_begin)*local_size+(j-col_begin)) = i+j;
         //*(C+(i-row_begin)*local_size+(j-col_begin)) = 0.0;
         
         //B[i-row_begin][j-col_begin] = i + j;
      }
   }

   // Start the timer
   double start_time;
   double compute_time=0.0;
   double comm_time=0.0;

   // Compute C = A * B
   int rootA, rootB;
   for (int k=0; k<sqrt_num_procs; k++) {
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Barrier(cartcomm);
      MPI_Barrier(row_comm);
      MPI_Barrier(col_comm);
      if (rank == 0) {
         start_time = MPI_Wtime();
      }

      // Broadcast
      if (k==coords[1]) {
         MPI_Bcast(A, local_size*local_size, MPI_DOUBLE, k, row_comm);
      } else {
         MPI_Bcast(bufferA, local_size*local_size, MPI_DOUBLE, k, row_comm);
      }
      if (k==coords[0]) {
         MPI_Bcast(B, local_size*local_size, MPI_DOUBLE, k, col_comm);
      } else {
         MPI_Bcast(bufferB, local_size*local_size, MPI_DOUBLE, k, col_comm);
      }

      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Barrier(cartcomm);
      MPI_Barrier(row_comm);
      MPI_Barrier(col_comm);
      if (rank == 0) {
         comm_time += MPI_Wtime()-start_time;
      }


      // Compute
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Barrier(cartcomm);
      MPI_Barrier(row_comm);
      MPI_Barrier(col_comm);
      if (rank == 0) {
         start_time = MPI_Wtime();
      }
      double flops = (double)size*(double)size*(double)size * (double) FLOP_CALIBRATION_FACTOR;
      SMPI_SAMPLE_FLOPS(flops) {
      }
      /*
      if ((coords[0] == k) && (coords[1] == k)) {
         mat_mul(A, B, C, local_size);
      } else if (coords[0] == k) {
         mat_mul(bufferA, B, C, local_size);
      } else if (coords[1] == k) {
         mat_mul(A, bufferB, C, local_size);
      } else {
         mat_mul(bufferA, bufferB, C, local_size);
      }
      */
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Barrier(cartcomm);
      MPI_Barrier(row_comm);
      MPI_Barrier(col_comm);
      if (rank == 0) {
         compute_time += MPI_Wtime()-start_time;
      }
   }

   if (0 == rank) {
      fprintf(stdout, "N = %d | p = %d | compute time: %.10lf seconds\n",
            size, num_procs, compute_time);
      /*
      fprintf(stdout, "N = %d | p = %d | comm time: %.10lf seconds\n",
            size, num_procs, comm_time);
            */
   }

   // Sum output for each process
   double sum = 0.0;
   for (i = 0; i < local_size; i++) {
      for (j = 0; j < local_size; j++) {
         sum += *(C+i*local_size+j);
      }
   }

   // Test total sum
   double ideal = (double)size*size*size*(size-1)*(size-1)*0.5;
   double total = 0.0;
   MPI_Reduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   if(rank == 0) {
      printf("The sum is %g\n", total);
      //printf("The ideal is %g\n", ideal);
      double err = total-ideal;
      //printf("The err is %g\n", err);
      if (err > E || err < -E) {
         printf("Error!\n");
      }
   }
#ifdef DEBUG
   printf("rank: %d, sum: %f\n", rank, sum);
   // for N=8, sum total = 12544
   // for N=4, sum total = 32*9=288
   // for N=16, sum total = 460800

   
   printf("Process rank %d (coordinates: %d, %d), block of A:\n", 
            rank, coords[0], coords[1]);
   print_mat(A, local_size);
   printf("Process rank %d (coordinates: %d, %d), block of B:\n", 
            rank, coords[0], coords[1]);
   print_mat(B, local_size);
   printf("Process rank %d (coordinates: %d, %d), block of C:\n", 
            rank, coords[0], coords[1]);
   print_mat(C, local_size);
#endif

// Clean-up
MPI_Barrier(MPI_COMM_WORLD);
MPI_Barrier(cartcomm);
MPI_Barrier(row_comm);
MPI_Barrier(col_comm);

MPI_Comm_free(&row_comm);
MPI_Comm_free(&col_comm);
SMPI_SHARED_FREE(A);
SMPI_SHARED_FREE(B);
SMPI_SHARED_FREE(C);
SMPI_SHARED_FREE(bufferA);
SMPI_SHARED_FREE(bufferB);
/*
free(A);
free(B);
free(C);
free(bufferA);
free(bufferB);
*/
MPI_Finalize();

return 0;
}
