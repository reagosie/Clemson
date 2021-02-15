#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 10

void *PrintHello(void *threadid)
{
   printf("Hello World from thread %d!\n", *(int*)threadid);
   pthread_exit(threadid);
}

int main (int argc, char **argv)
{
  pthread_t threads[NTHREADS];
  int i,j;
  long errCheck;

  for(i = 0; i < NTHREADS; i++){
    errCheck = pthread_create(&(threads[i]), NULL, PrintHello, (void *)&i);
    if (errCheck){
       printf("ERROR; return code from pthread_create() is %ld\n", errCheck);
       exit(-1);
    }
  }
  for(j = 0; j < NTHREADS; j++){
    pthread_join(threads[j], NULL);
    printf("Thread %d returned %ld\n", j, errCheck);
  }
  printf("Main thread done.\n");
  return 0;
}
