//     *********************************
//     *    Matrix Multiply Project    *
//     *                               *
//     *********************************

//     ** MAIN PROGRAM  **


//     *************************************************
//     ** Any changes you make to this code must      **
//     ** maintain the correctness of the matrix      **
//     ** multiply computed by the original version.
//     **					      **
//     ** An implementation with incorrect results for**
//     ** matrix C earns zero point for this project. **
//     **
//     ** To print matrix compile with -DPRINT_MATRIX **
//     ** e.g., gcc -DPRINT_MATRIX                    **
//     **                                             **
//     ** A sample Makefile is provided.
//     ** You may assume m = n = k for your matrices  **
//     *************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double **dmatrix(int nrl,int nrh,int ncl,int nch);
void nerror(char *error_text);

//function get_time() to get elapsed nanoseconds
static long get_time() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

/*
//function get_time() to get elapsed microseconds
unsigned long get_time() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
        return time_in_micros;
}*/

int main(int argc, char** argv)  {

      register int l,m,n,k;
      register int i,j;
      double temp;
      double **A, **B, **C;


     //  ****************************************************
     //  * The following allows matrix parameters to be     *
     //  * entered on the command line to take advantage    *
     //  * of dynamically allocated memory.  You may modify *
     //  * or remove it as you wish.                        *
     //  ****************************************************

     if (argc != 4) {
       nerror("Usage:  <executable> <m-value> <n-value> <k-value>");
     }
      m = atoi(argv[1]);
      n = atoi(argv[2]);
      k = atoi(argv[3]);

      // *********************************************************
      // * Call the dmatrix() subroutine to dynamically allocate *
      // * storage for the matrix sizes specified by m, n, and k *
      // *********************************************************

      A=dmatrix(0,m-1,0,k-1);
      B=dmatrix(0,k-1,0,n-1);
      C=dmatrix(0,m-1,0,n-1);

      // *********************************************************
      //  * Initialize matrix elements so compiler does not      *
      //  * optimize out                                         *
      // *********************************************************

      for(j=0;j<k;j++) {
        for(i=0;i<m;i++) {
          A[i][j] = i+j+4.0;
        }
      }

      for(j=0;j<n;j++) {
        for(i=0;i<k;i++) {
          B[i][j] = i+j+5.0;
        }
      }

      for(j=0;j<n;j++) {
        for(i=0;i<m;i++) {
          C[i][j] = 0.0;
        }
      }

      //create my variables for blocking and unrolling
      //also do division/modulus to figure out remainder
      register int rem = m % 4;
      register int rounds = m / 4;
      register int a = 0;

      // ******************************
      // * Start embedded timing here *
      // ******************************
      long start_time = get_time();
      // **********************************
      // * Perform simple matrix multiply *
      // **********************************

      for(j=0;j<n;j+=4) {
        for(l=0;l<k;l++) {
          for(i=0;i<m;i+=4) {
            for(a=0;a<4;a++) {
              if(rem >= 0){
                if(rem == 0 || rounds != 0){
                  C[i+a][j] = C[i+a][j] + B[l][j]*A[i+a][l];
                  C[i+a][j+1] = C[i+a][j+1] + B[l][j+1]*A[i+a][l];
                  C[i+a][j+2] = C[i+a][j+2] + B[l][j+2]*A[i+a][l];
                  C[i+a][j+3] = C[i+a][j+3] + B[l][j+3]*A[i+a][l];
                }
                else if(rem = 1){
                  C[i][j] = C[i][j] + B[l][j]*A[i][l];
                  C[i][j+1] = C[i][j+1] + B[l][j+1]*A[i][l];
                  C[i][j+2] = C[i][j+2] + B[l][j+2]*A[i][l];
                  C[i][j+3] = C[i][j+3] + B[l][j+3]*A[i][l];
                  i++;
                  rem = -1;
                  break;
                }
                else{
                  C[i][j] = C[i][j] + B[l][j]*A[i][l];
                  C[i][j+1] = C[i][j+1] + B[l][j+1]*A[i][l];
                  C[i][j+2] = C[i][j+2] + B[l][j+2]*A[i][l];
                  C[i][j+3] = C[i][j+3] + B[l][j+3]*A[i][l];
                  i++;
                  rem--;
                }
              }
            }
          rounds--;
          }
        }
      }
      // ******************************
      // * Stop embedded timing here  *
      // ******************************
      fprintf(stdout, "TimeTaken = %ld nanoseconds.\n", get_time() - start_time);
      // **************************************************
      // * Print out a 10 x 10 matrix for testing only    *
      // * Comment out when timing                        *
      // **************************************************

 #ifdef PRINT_MATRIX
      fprintf(stdout, "Here is the matrix A:\n\n");
      for(i=0;i<m;i++) {
        for(j=0;j<k;j++) {
          fprintf(stdout, "%10.2f ",A[i][j]);
        }
        fprintf(stdout, "\n");
      }
      fprintf(stdout, "Here is the matrix B:\n\n");
      for(i=0;i<k;i++) {
        for(j=0;j<n;j++) {
          fprintf(stdout, "%10.2f",B[i][j]);
        }
        fprintf(stdout, "\n");
      }
      fprintf(stdout, "Here is the matrix C:\n\n");
      for(i=0;i<m;i++) {
        for(j=0;j<n;j++) {
          fprintf(stdout, "%10.2f",C[i][j]);
        }
        fprintf(stdout, "\n");
      }
#endif

}

//     **  END MAIN PROGRAM  **

//     ********************************************************
//     *******    BEGIN SUBROUTINES    ************************
//     ********************************************************

double **dmatrix(int nrl,int nrh,int ncl,int nch)
// Allocates a double matrix with range [nrl..nrh][ncl..nch]
{
  int i;
  double **m;

//    Allocate pointers to rows
  m=(double **) malloc((unsigned)(nrh-nrl+1)*sizeof(double *));
  if (!m) nerror("allocation failure in malloc in dmatrix()");
  m -= nrl;
//    Allocate rows and set pointers to them
  for(i=nrl;i<=nrh;i++) {
    m[i]=(double*) malloc((unsigned) (nch-ncl+1)*sizeof(double));
    if (!m[i]) nerror("allocaion failure in malloc in dmatrix()");
    m[i] -= ncl;
  }
  return m;
}

void nerror(char *error_text)
{
  void exit();
  fprintf(stderr, "Run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"Exiting...\n");
  exit(1);
}
