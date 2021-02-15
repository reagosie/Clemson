#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
   printf("Hello World! It's me, thread #%d\n", *(int*)threadid);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int ret;
   int t;
   for(t = 0; t < NUM_THREADS; t++){
      printf("In main: creating thread %d\n", t);
      ret = pthread_create(&threads[t], NULL, PrintHello, (void *)&t);
      if (ret){
         printf("ERROR; return code from pthread_create() is %d\n", ret);
         exit(-1);
      }
   }

   //need to join threads here
   for(t = 0; t < NUM_THREADS; t++){
        pthread_join(threads[t], NULL);
        printf("Thread %ld joined\n", threads[t]);
   }
return 0;
}
