#include <stdlib.h>
#include <stdio.h>

//we are assuming that the 2D array will always be of size 1000 x 1000
int ROWS = 1000, COLS = 1000;

/* 10 colors plus white are defined.  Many more are possible. */
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

void CopyNewToOld(float new[ROWS][COLS], float old[ROWS][COLS]){
  for(int i = 1; i < ROWS-1; i++){
    for(int j = 1; j < COLS-1; j++){
      old[i][j] = new[i][j];
    }
  }
}

void CalculateNew(float new[ROWS][COLS], float old[ROWS][COLS], int xsource, int ysource){
  /*The location and temperature of the heat source will need to stay fixed at all times. The
  CalculateNew function will have code that is similar to:*/
  for(int i = 1; i < ROWS-1; i++){
    for(int j = 1; j < COLS-1; j++){
      new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
    }
  }
}

void PrintGridtoFile(float new[ROWS][COLS]){
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
         if(new[i][j] > 250){color = 0;}
         else if(new[i][j] > 180 && new[i][j] < 250){color = 1;}
         else if(new[i][j] > 120 && new[i][j] < 180){color = 2;}
         else if(new[i][j] > 80 && new[i][j] < 120){color = 3;}
         else if(new[i][j] > 60 && new[i][j] < 80){color = 4;}
         else if(new[i][j] > 50 && new[i][j] < 60){color = 5;}
         else if(new[i][j] > 40 && new[i][j] < 50){color = 6;}
         else if(new[i][j] > 30 && new[i][j] < 40){color = 7;}
         else if(new[i][j] > 20 && new[i][j] < 30){color = 8;}
         else{color = 9;}
         fprintf(fp, "%s ", colors[color] );
       }
    fprintf(fp, "\n");
  }
  fclose(fp);
  /* Convert the pnm file to a format that is more easily viewed
    in a web browser. */
  /*   system("convert c.pnm c.png"); */
  system("convert c.pnm c.gif");   /* png not supported on comp */
}

int main(int argc, char* argv[]){
  int xsource = 0, ysource = 0, iterations = 1000;
  float new[ROWS][COLS], old[ROWS][COLS];
  //initialize first state of the room (new and old)...
  //initialize everything to 20 degrees
  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      old[i][j] = 20;
      new[i][j] = 20;
    }
  }
  //top edge
  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      //make middle 40% of top edge be the fireplace (300 degrees)
      if(j == 0 && (i > (ROWS*0.3) && i < (ROWS*0.7))){
        old[i][j] = 300;
        new[i][j] = 300;
      }
    }
  }

  for(int i = 0; i < iterations; i++){
    CopyNewToOld(new, old);
    CalculateNew(new, old, xsource, ysource);
  }//close big for

  PrintGridtoFile(new);
  return 0;
}
