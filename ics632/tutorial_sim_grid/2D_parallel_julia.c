#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb);
int write_bmp_header(FILE *f, int width, int height);


/*
 * compute_julia_pixel(): compute RBG values of a pixel in a 
 *                        particular Julia set image.
 *
 *  In:
 *	(x,y):            pixel coordinates	
 *	(width, height):  image dimensions
 *	tint_bias:	  a float to "tweak" the tint (1.0 is "no additional tint")
 *  Out:
 *	rgb: an already-allocated 3-byte array into which R, G, and B 
 *	     bytes are written.
 *
 *  Return:
 *  	0 in success, -1 on failure
 *
 */


int compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb) {

  // Check coordinates
  if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
    fprintf(stderr,"Invalid (%d,%d) pixel coordinates in a %d x %d image\n", x, y, width, height);
    return -1;
  }

  // "Zoom in" to a particular section of the Mandelbrot set
  float X_MIN = -1.6, X_MAX = 1.6, Y_MIN = -0.9, Y_MAX = +0.9;
  float float_y = (Y_MAX - Y_MIN) * (float)y / height + Y_MIN ;
  float float_x = (X_MAX - X_MIN) * (float)x / width  + X_MIN ;

  // Point that defines the Julia set
  float julia_real = -.79;
  float julia_img = .15;

  // Maximum of iteration before we determine "did not converge"
  int max_iter = 300;

  // Computing the complex series convergence
  float real=float_y, img=float_x;
  int num_iter = max_iter;
  while (( img * img + real * real < 2 * 2 ) && ( num_iter > 0 )) {
    float xtemp = img * img - real * real + julia_real;
    real = 2 * img * real + julia_img;
    img = xtemp;
    num_iter--;
  }
  //
  // Paint pixels based on how many iterations were used, using some funky colors
  float color_bias = (float) num_iter / max_iter;

  rgb[0] = (num_iter == 0 ? 200 : - 500.0 * pow(tint_bias, 1.2) *  pow(color_bias, 1.6)); 
  rgb[1] = (num_iter == 0 ? 100 : -255.0 *  pow(color_bias, 0.3));
  rgb[2] = (num_iter == 0 ? 100 : 255 - 255.0 * pow(tint_bias, 1.2) * pow(color_bias, 3.0));

  return 0;
}


/* write_bmp_header():
 *
 *   In:
 *      f: A file open for writing ('w') 
 *      (width, height): image dimensions
 *   
 *   Return:
 *      0 on success, -1 on failure
 *
 */

int write_bmp_header(FILE *f, int width, int height) {

  unsigned int adjusted_width = width + (width % 4 == 0 ? 0 : (4 - width % 4));

  // Define all fields in the bmp header
  char id[2] = "BM";
  unsigned int filesize = 54 + (int)(adjusted_width * height * 3 * sizeof(char));
  short reserved[2] = {0,0};
  unsigned int offset = 54;

  unsigned int size = 40;
  unsigned short planes = 1;
  unsigned short bits = 24;
  unsigned int compression = 0;
  unsigned int image_size = width * height * 3 * sizeof(char);
  int x_res = 0;
  int y_res = 0;
  unsigned int ncolors = 0;
  unsigned int importantcolors = 0;

  // Write the bytes to the file, keeping track of the
  // number of written "objects"
  size_t ret = 0;
  ret += fwrite(id, sizeof(char), 2, f);
  ret += fwrite(&filesize, sizeof(int), 1, f);
  ret += fwrite(reserved, sizeof(short), 2, f);
  ret += fwrite(&offset, sizeof(int), 1, f);
  ret += fwrite(&size, sizeof(int), 1, f);
  ret += fwrite(&width, sizeof(int), 1, f);
  ret += fwrite(&height, sizeof(int), 1, f);
  ret += fwrite(&planes, sizeof(short), 1, f);
  ret += fwrite(&bits, sizeof(short), 1, f);
  ret += fwrite(&compression, sizeof(int), 1, f);
  ret += fwrite(&image_size, sizeof(int), 1, f);
  ret += fwrite(&x_res, sizeof(int), 1, f);
  ret += fwrite(&y_res, sizeof(int), 1, f);
  ret += fwrite(&ncolors, sizeof(int), 1, f);
  ret += fwrite(&importantcolors, sizeof(int), 1, f);

  // Success means that we wrote 17 "objects" successfully
  return (ret != 17);
}

/*
 * main() function
 */

int main(int argc, char **argv) {

  MPI_Init(&argc, &argv);

  // Parse command-line arguments
  int height;
  if ((argc != 2) || (sscanf(argv[1],"%d",&height) != 1)) {
    fprintf(stderr,"Usage: %s <height (int)>\n",argv[0]);
    exit(1);
  }
  int width = height * 2;

  int myrank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  int num_procs_1d = sqrt(num_procs);
  int hRemainder =height % num_procs_1d;
  int wRemainder =width % num_procs_1d;

  int myRank[2];
  /*myRank(r,c) For example: 9 processors
  |-----------------------|
  |(0,0) | (0, 1) | (0,2) |
  |-----------------------|
  |(1,0) | (1, 1) | (1,2) |
  |-----------------------|
  |(2,0) | (2, 1) | (2,2) |
  |-----------------------|
  */
  myRank[0] = myrank/num_procs_1d;  //row
  myRank[1] = myrank%num_procs_1d;  //column

  int rowStart = myRank[0]*height/num_procs_1d; 
  int rowStop = (myRank[0]+1)*height/num_procs_1d+(myRank[0]<hRemainder)-1;
  int colStart = myRank[1]*width/num_procs_1d;
  int colStop = (myRank[1]+1)*width/num_procs_1d+(myRank[1]<wRemainder)-1;
  printf("[Process rank %d]: my 2-D rank is (%d, %d), my tile is [%d:%d, %d:%d]\n", myrank, myRank[0], myRank[1], rowStart, rowStop, colStart, colStop);
  //printf("width=%d, num_procs_1d=%d\n", width, num_procs_1d);

  // Allocate array
  //unsigned char *pixels = (unsigned char *)calloc(height * width * 3, sizeof(char));
  int myHeight = rowStop-rowStart+1;
  int myWidth = colStop-colStart+1;
  unsigned char *pixels = (unsigned char *)calloc(myHeight * myWidth * 3, sizeof(char));

  // Compute all pixels
  int y,x;
  for (y=0; y < myHeight; y++) {
    for (x=0; x < myWidth; x++) {
      compute_julia_pixel(colStart+x, rowStart+y, width, height, myrank*myrank, &(pixels[y * 3 * myWidth + x * 3]));
    }
  }


  // Open file for writing
  FILE *f;
  int data[1];
  data[0] = 1; //random numer, does not matter in this case?
  MPI_Status status;

  // Write the pixels
  int p;
  int preRank,nextRank;//previous process, next process
  for (y=0; y < myHeight; y++) {
     for (x=0; x < myWidth; x++) {
        if(myrank ==0 && y==0 && x==0){  //rank 0 and first row, open the picture
           f = fopen("./julia.bmp", "w");
           // Write header
           if (write_bmp_header(f, width, height)) {
              perror("Error writing header: ");
              exit(0);
           }
           fwrite(&(pixels[y * 3 * myWidth + x * 3]), sizeof(char), 3, f);
        }else{
           if(x == colStart){
              if(myRank[1] == 0 && y != rowStart) preRank = myrank+num_procs_1d-1;
              else preRank = myrank-1;
              MPI_Recv(data,1,MPI_INT, preRank,0,MPI_COMM_WORLD, &status);
              f = fopen("./julia.bmp", "a");
           }

           //printf("rank=%d, x=%d, y=%d, myWidth=%d\n", myrank, x, y, myWidth);
           //////////////////////////////////////////////
           //segmentation fault at rank 1, first pixels//
           //////////////////////////////////////////////
           if(myrank == 1) {
              printf("x=%d, y=%d\n", x, y);
              printf("%c\n", pixels[y*3*myWidth+x*3]);
           }
           fwrite(&(pixels[y * 3 * myWidth + x * 3]), sizeof(char), 3, f);

           // padding in case of an even number of pixels per row
           unsigned char padding[3] = {0,0,0};
           fwrite(padding, sizeof(char), ((myWidth * 3) % 4), f);

           if(x == colStop){

              // Close the file
              fclose(f);

              if(myrank != num_procs-1 || y!=myHeight-1 || x!=myWidth-1) {
                 if(myRank[1]==num_procs_1d-1 && y!=rowStop) nextRank = myrank-num_procs_1d+1;
                 else nextRank = myrank+1;
                 MPI_Send(data,1,MPI_INT, nextRank,0,MPI_COMM_WORLD);
              }
           }
        }
     }
  }

  MPI_Finalize();
  return 0;
}


