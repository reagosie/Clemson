// file source: https://gist.github.com/rtv/4989304

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


/* Compile like this:
 gcc -pthread ex1.c
*/

//initialize the number of threads we are going to be working with
const size_t NUMTHREADS = 20;

//declare and initialize variable to keep track of how many threads have completed
int done = 0;
//declare and initialize mutex and cond objects
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//this function simulates each thread performing some job
void* ThreadEntry( void* id ){
  const int myid = (long)id; // force the pointer to be a 64bit integer

  //this is how many times we will iterate through our "working" loop
  const int workloops = 5;
  //loop to simulate work being done
  for( int i=0; i<workloops; i++ ){
      //print statement to show progress throughout the imaginary job
      printf( "[thread %d] working (%d/%d)\n", myid, i, workloops );
      sleep(1); // simulate doing some costly work
  }

  //lock our mutex object
  pthread_mutex_lock( &mutex );

  //increment # of threads that have completed
  done++;
  //print statement to show that this thread has completed all its work
  printf( "[thread %d] done is now %d. Signalling cond.\n", myid, done );
  //2 signals to restart one of the threads that's waiting on cond and to unlock our mutex object
  pthread_cond_signal( &cond );
  pthread_mutex_unlock( & mutex );

  return NULL;
}

//main function
int main( int argc, char** argv )
{
  //print statement to signal that the main, overarching thread is starting
  puts( "[thread main] starting" );
  //declare thread array of size that we initialized earlier
  pthread_t threads[NUMTHREADS];

  //loop to create our 20 (NUMTHREADS) threads and number them 1-20 but with (void*)(long) typecast
  for( int t=0; t<NUMTHREADS; t++ )
    pthread_create( &threads[t], NULL, ThreadEntry, (void*)(long)t );

  //lock our mutex variable
  pthread_mutex_lock( &mutex );

  //while the # of completed threads is less than the total # of threads...
  while( done < NUMTHREADS ){
      //print statement to say that there are still threads which are not yet completed
      printf("[thread main] done is %d which is < %d so waiting on cond\n", done, (int)NUMTHREADS);
      //wait until cond is signalled
      pthread_cond_wait( & cond, & mutex );
      //print that cond was signalled and that the main thread was woken up
      puts( "[thread main] wake - cond was signalled." );
  }

  //print that our main thread has completed because all of it's children threads have completed
  printf( "[thread main] done == %d so everyone is done\n", (int)NUMTHREADS );
  //unlock our mutex variable
  pthread_mutex_unlock( & mutex );

  return 0;
}
