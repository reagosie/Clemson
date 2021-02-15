#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]){
  int i, size;
  int rank; //number of processes we're requesting from palmetto cluster
  double x, y, pi;
  int count = 0, local_count = 0;
  int samples = atoi(argv[1]);
  MPI_Status status;
  double startTime = MPI_Wtime();

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int equalShare = samples/size; //equalShare is the number of loops each process will take care of

  for(i = 0; i < equalShare; i++){
    x = (double) rand() / RAND_MAX;
    y = (double) rand() / RAND_MAX;
    if(x*x + y*y <= 1){local_count++;}
  }

  printf("Local count on process %d is %d\n", rank, local_count);

  /*if(rank != 0){
    //dest = 0;
    //MPI_Send(&local_count, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
  }
  else{
    count = local_count;
    for(i = 1; i < size; i++){
      //MPI_Recv(&local_count, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);*/
      MPI_Reduce(&local_count, &count, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      /*//count += local_count;
    }
  }*/

  printf("Count on process %d is %d\n", rank, count);
  pi = 4.0 * (double)count / (double)samples;
  double endTime = MPI_Wtime();

  if(rank == 0){
    printf("Count = %d, Samples = %d, Estimate of pi = %7.5f\n", count, samples, pi);
    printf("Calculation time: %1.4f seconds\n", endTime-startTime);
  }

  MPI_Finalize();
  return 0;
}
