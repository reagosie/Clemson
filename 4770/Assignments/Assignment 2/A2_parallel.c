#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

//we are assuming that the 2D array will always be of size 1000 x 1000
int ROWS = 1000, COLS = 1000;

/* 10 colors plus white are defined.*/
#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

void CopyNewToOld(int rowSize, float new[rowSize][COLS], float old[rowSize][COLS]){
  for(int i = 1; i < rowSize; i++){
    for(int j = 1; j < COLS-1; j++){
      old[i][j] = new[i][j];
    }
  }
}

void CalculateNew(int rowSize, float new[rowSize][COLS], float old[rowSize][COLS], int xsource, int ysource){
  /*The location and temperature of the heat source will need to stay fixed at all times. The
  CalculateNew function will have code that is similar to:*/
  for(int i = 1; i < rowSize-1; i++){
    for(int j = 1; j < COLS-1; j++){
      new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
    }
  }
}

void PrintGridtoFile(float finalGrid[ROWS][COLS]){
  FILE * fp;
  int numcolors = 10;
  int color;
  /* Colors are list in order of intensity */
  char * colors[10] = { RED, ORANGE, YELLOW, LTGREEN, GREEN,
                           LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };

  /* The image will be 300 pixels wide and 300 pixels tall */
  int linelen = COLS;
  int numlines = ROWS;
  int i, j;

  /* The pnm filename is hard-coded.  */
  fp = fopen("c.pnm", "w");
  /* Print the P3 format header */
  fprintf(fp, "P3\n%d %d\n15\n", linelen, numlines);

  /* Print 300 lines of colors. ASCII makes this easy.           */
  /* Each %s (color string) is a single pixel in the final image */
  for (j=0; j<numlines; j++){
    for (i=0; i<linelen; i++){
         if(finalGrid[i][j] > 250){color = 0;}
         else if(finalGrid[i][j] > 180 && finalGrid[i][j] < 250){color = 1;}
         else if(finalGrid[i][j] > 120 && finalGrid[i][j] < 180){color = 2;}
         else if(finalGrid[i][j] > 80 && finalGrid[i][j] < 120){color = 3;}
         else if(finalGrid[i][j] > 60 && finalGrid[i][j] < 80){color = 4;}
         else if(finalGrid[i][j] > 50 && finalGrid[i][j] < 60){color = 5;}
         else if(finalGrid[i][j] > 40 && finalGrid[i][j] < 50){color = 6;}
         else if(finalGrid[i][j] > 30 && finalGrid[i][j] < 40){color = 7;}
         else if(finalGrid[i][j] > 20 && finalGrid[i][j] < 30){color = 8;}
         else{color = 9;}
         fprintf(fp, "%s ", colors[color] );
       }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

int main(int argc, char* argv[]){
  int rank, size;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int xsource = 0, ysource = 0, iterations = 1000, rowSize, sendbuf, i, j;
  //declare new and old 2D arrays of different size based on their rank
  if(rank==0){
    rowSize = (ROWS/size) + 1;
  }
  else if(rank==size-1){
    rowSize = (ROWS/size) + 1;
  }
  else{
    rowSize = (ROWS/size) + 2;
  }

  float new[rowSize][COLS];
  float old[rowSize][COLS];

  //initialize first state of the room (new and old)...
  //initialize everything to 20 degrees
  for(int i = 0; i < rowSize; i++){
    for(int j = 0; j < COLS; j++){
      old[i][j] = 20;
      new[i][j] = 20;
    }
  }
  //top edge
  for(int i = 0; i < rowSize; i++){
    for(int j = 0; j < COLS; j++){
      //make middle 40% of top edge be the fireplace (300 degrees)
      if(j == 0 && (i > (rowSize*0.3) && i < (rowSize*0.7))){
        old[i][j] = 300;
        new[i][j] = 300;
      }
    }
  }


  for(int i = 0; i < iterations; i++){
      for(int i = 0; i < 1; i++){
    //update ghost rows
    if(rank==0){
      MPI_Send(new[rowSize-2], COLS, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD);
      MPI_Recv(new[rowSize-1], COLS, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD, &status);
    }
    else if(rank != size-1){
      MPI_Send(new[1], COLS, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD);
      MPI_Recv(new[0], COLS, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD, &status);

      MPI_Recv(new[rowSize-1], COLS, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD, &status);
      MPI_Send(new[rowSize-2], COLS, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD);
    }
    else if(rank == size-1){
      MPI_Send(new[1], COLS, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD);
      MPI_Recv(new[0], COLS, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

    CopyNewToOld(rowSize, new, old);
    CalculateNew(rowSize, new, old, xsource, ysource);
  }

 //The following lines 145-154 were not working properly at the time of submission. These lines
 //were suggested by Dr. Jin but we could not get them to work. They are commented out
 //because the program successfully finishes execution without them, but not with them
 //included.
 /*
    if(rank==0){
        float finalGrid[ROWS][COLS];
        for(i = 1; i < size; i++){
          MPI_Recv(finalGrid, (COLS * (rowSize+1)), MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
        }
        PrintGridtoFile(finalGrid);
      }
    else{
        MPI_Send(new, (COLS * rowSize), MPI_FLOAT, rank, 0, MPI_COMM_WORLD);
    }
 */
  MPI_Finalize();
  return 0;
}
