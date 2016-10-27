#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

// Define neighbors
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

///////////////////////////////////////////////////////
//// program_abort() and print_usage() functions //////
///////////////////////////////////////////////////////

static void program_abort(char *exec_name, char *message);
static void print_usage();

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

///////////////////////////
////// Main function //////
///////////////////////////

int main(int argc, char *argv[])
{
   int i,j;
   int size;

   // Parse command-line arguments (not using getopt because not thread-safe
   // and annoying anyway). The code below ignores extraneous command-line
   // arguments, which is lame, but we're not in the business of developing
   // a cool thread-safe command-line argument parser.

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
   } else if (size % num_procs != 0) {
      sprintf(message, "Invalid matrix dimention, use value that dividable by %d\n",sqrt_num_procs);
      program_abort(NULL,message);
   }

   // Allocate matrix buffer
   int checksum;
   int local_size= size/sqrt_num_procs;
   float *A[local_size];
   float *B[local_size];
   float *C[local_size];
   for (i=0; i<local_size; i++) {
      if ((A[i] = malloc(sizeof(char) * local_size)) == NULL ||
            (B[i] = malloc(sizeof(float) * local_size)) == NULL ||
            (C[i] = malloc(sizeof(float) * local_size)) == NULL ) {
         program_abort(argv[0],"Out of memory!");
      }
   }

   // Create cartesian virtual topology, get rank, coordinates, neighbor ranks
   int dims[2] = {sqrt_num_procs, sqrt_num_procs};
   int periods[2] = {0, 0};
   int reorder = 0;
   int nbrs[4];
   int coords[2];
   MPI_Comm cartcomm;

   MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm);
   MPI_Comm_rank(cartcomm, &rank);
   MPI_Cart_coords(cartcomm, rank, 2, coords);
   /*
      MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN]);
      MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT]);

      printf("rank= %d coords= %d %d neighbors(u, d, l, r)= %d %d %d %d\n",
      rank, coords[0], coords[1], nbrs[UP], brs[DOWN], nbrs[LEFT], nbrs[RIGHT]);
      */

   // Init matrix
   int row_begin, row_end;
   int col_begin, col_end;
   row_begin = coords[0] * local_size;
   row_end = row_begin + local_size - 1;
   col_begin = coords[1] * local_size;
   col_end = col_begin + local_size - 1;
   //printf("rank=%d, row from %d to %d, col from %d to %d\n", rank, row_begin, row_end, col_begin, col_end);
   for (i = row_begin; i <= row_end; i++) {
      for (j = col_begin; j <= col_end; j++) {
         A[i-row_begin][j-col_begin] = i;
         B[i-row_begin][j-col_begin] = i + j;
      }
   }

   // Test output
   printf("Process rank %d (coordinates: %d, %d), block of A:\n", rank, coords[0], coords[1]);
   for (i = 0; i < local_size; i++) {
      for (j = 0; j < local_size; j++) {
         printf("%.1f ", A[i][j]);
      }
      printf("\n");
   }
   printf("Process rank %d (coordinates: %d, %d), block of B:\n", rank, coords[0], coords[1]);
   for (i = 0; i < local_size; i++) {
      for (j = 0; j < local_size; j++) {
         printf("%.1f ", B[i][j]);
      }
      printf("\n");
   }

   /*
   // Start the timer
   double start_time;
   MPI_Barrier(MPI_COMM_WORLD);
   if (rank == 0) {  
   start_time = MPI_Wtime();
   }

   // All processes send checksums back to the root, which checks for consistency
   char all_ok = 1;
   if (0 == rank) {
   for (j = 1; j < num_procs; j++) {
   int received_checksum;
   MPI_Recv(&received_checksum, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
// Print a single message in case of a mismatch, but continue
// receiving other checksums to ensure that all processes
// reach the MPI_Finalize()
if ((all_ok == 1) && (checksum != received_checksum)) {
fprintf(stderr,"\t** Non-matching checksum! **\n");
all_ok = 0;
break;
}
}
} else {
int checksum=0;
for (j = 0; j < NUM_BYTES; j++) {
checksum += buffer[j];
}
MPI_Send(&checksum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
}

// Print out bcast implementation name and wall-clock time, only if the bcast was successful
MPI_Barrier(MPI_COMM_WORLD);
if ((0 == rank) && (all_ok)) {
fprintf(stdout,"implementation: %s | chunksize: %d |  time: %.3lf seconds\n",
bcast_implementation_name, 
chunk_size,
MPI_Wtime() - start_time);
}
*/

// Clean-up
MPI_Barrier(MPI_COMM_WORLD);
for (i=0; i<local_size; i++) {
   free(A[i]);
   free(B[i]);
   free(C[i]);
}
MPI_Finalize();

return 0;
}
