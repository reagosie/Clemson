/*********************************************************
* Module Name:  Tools common routines 
*
* File Name:    utils..c
*
*
*********************************************************/
#include "UDPEcho.h"
//#include "portable_endian.h"
//#include <endian.h> 
/* needed by the htonll and ntohll routines */

char Version[] = "3.0";   

double getTime(int);
double getTime1();
double convertTS2D(struct timespec *t);

int delay(int64_t ns);
int gettimeofday_benchmark();
  // 1:getTimeofDay; 
  // 2:clock_gettime with CLOCK_REALTIME
  // 3:clock_gettime with CLOCK_PROCESS_CPU_TIME


void die(const char *msg) {
  if (errno == 0) {
    /* Just the specified message--no error code */
    puts(msg);
  } else {
    /* Message WITH error code/name */
    perror(msg);
  }
  printf("Die message: %s \n", msg);
  
  /* DIE */
  exit(EXIT_FAILURE);
}

long getMicroseconds(struct timeval *t) {
  return (t->tv_sec) * 1000000 + (t->tv_usec);
}

//Returns timeval in seconds with usecond precision
double convertTimeval(struct timeval *t) {
  return ( t->tv_sec + ( (double) t->tv_usec)/1000000 );
}

//Returns time difference in microseconds 
long getTimeSpan(struct timeval *start_time, struct timeval *end_time) {
  long usec2 = getMicroseconds(end_time);
  long usec1 = getMicroseconds(start_time);
  return (usec2 - usec1);
}

double timestamp() 
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) { 
    die("the sky is falling!"); 
  }
  return ((double)tv.tv_sec + ((double)tv.tv_usec / 1000000));
}

void setUnblockOption(int sock, char unblock) {
  int opts = fcntl(sock, F_GETFL);
  if (unblock == 1)
    opts |= O_NONBLOCK;
  else
    opts &= ~O_NONBLOCK;
  fcntl(sock, F_SETFL, opts);
}

void sockBlockingOn(int sock) { setUnblockOption(sock, 0); }
void sockBlockingOff(int sock) { setUnblockOption(sock, 1); }


const int bsti = 1;  // Byte swap test integer
bool is_bigendian()
{
bool rc = false;

  rc =  (*(char*)&bsti) == 0;

  return rc;
}


/*************************************************************
*
* Function: void swapbytes(void *_object, size_t size)
* 
* Summary: In-place swapping of bytes to match endianness of hardware
*
* Inputs:
*   *object : memory to swap in-place
*   size   : length in bytes
*           
*
* outputs:  
*     updates caller's object data
*
* notes: 
*    
*   Timeval struct defines the two components as long ints
*         The following nicely printers: 
*         printf("%ld.%06ld\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
*
***************************************************************/
void swapbytes(void *_object, size_t size)
{
  unsigned char *start, *end;
//TODO:  replace with preprocessor macro
  if(!is_bigendian())
  {
    for ( start = (unsigned char *)_object, end = start + size - 1; start < end; ++start, --end )
    {
      unsigned char swap = *start;
      *start = *end;
      *end = swap;
    }
   }
}

/*************************************************************
*
* Function: uint64_t htonll (uint64_t InAddr) 
* 
* Summary:  equivalent to htonl but operates on long long which
*           is assumed to be uint64_t 
*
* Inputs:
*   uint64_t InAddr :  callers 64 bit data
*           
*
* outputs:  
*   returns the InAddr in network byte (Big Endian) format
*
* notes: Returns all 1's on error
*    
***************************************************************/
uint64_t htonll (uint64_t InAddr) 
{
  uint64_t rvalue = InAddr;
//  swapbytes((void *)(&rvalue),size(uint64_t));

//TODO:  replace with preprocessor macro
  if(!is_bigendian())
  {
    rvalue = htobe64(rvalue);
  }
  return rvalue;

}

/***********************************************************
* Function: double getCurTimeD() 
*
* Explanation:  This returns the wall time using 
*               CLOCK_REALTIME as the clock source.
*               Should do the same as timestamp() or
*                wallClockTime(), but possibly with
*               more precision.
*
* inputs: none
*
* outputs:
*    returns the wall time as a double representing 
*     the wall clock time  in seconds  with nanosecond precision
*
* notes: 
*     TAG WALLCLOCK
*
***********************************************************/
double getCurTimeD() 
{
  double timestamp = -1.0;
  struct timespec ts;
  int rc = NOERROR;

  rc = clock_gettime(CLOCK_REALTIME, &ts);

  if (rc==NOERROR) { 
      timestamp = ( (double)ts.tv_sec +  (double) (((double)ts.tv_nsec)/1000000000) );
  } else {
    perror("getCurTimeD:  HARD error on clock_gettime\n");
  }

  return(timestamp);
}


/***********************************************************
* Function: double getCurTime(struct timespec *ts) 
*
* Explanation:  This returns the wall time using 
*               CLOCK_REALTIME as the clock source.
*
* inputs: 
*       struct timespec *ts : callers timespec that is to be filed in.
*
* outputs:
*    returns the wall time in seconds  with nanosecond precision
*
* notes: 
*     TAG WALLCLOCK 
*
***********************************************************/
double getCurTime(struct timespec *ts) 
{
  double timestamp = -1.0;
  int rc = NOERROR;

  //likely to use CLOCK_REALTIME 
  rc = clock_gettime(CLOCK_REALTIME, ts);

  if (rc==NOERROR) { 
      timestamp = ( (double)ts->tv_sec +  (double) (((double)ts->tv_nsec)/1000000000) );
  } else {
    printf("getCurTime:  HARD error on clock_gettime,  errno:%d \n", errno);
    perror("getCurTimeD:  HARD error on clock_gettime\n");
  }

  return(timestamp);

}

/***********************************************************
* Function:  double getTimestampD() 
*
* Explanation:  This returns  a timestamp using 
*               the MONOTONIC_RAW clock source
*               Same as getTimestamp but without the ts param               
*
* inputs: 
*
* outputs:
*     returns a double representing a timestamp in seconds  with nanosecond precision
*
* notes: 
*
*     TAG TIMESTAMP
***********************************************************/
double getTimestampD() 
{
  double timestamp = -1.0;
  struct timespec ts;
  int rc = NOERROR;

  rc = clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  if (rc==NOERROR) { 
      timestamp = (double)ts.tv_sec + ((double)ts.tv_nsec)/1000000000;
  } else {
    perror("getTimestampD():  HARD error on clock_gettime\n");
    rc = ERROR;
  }

  if (rc == ERROR)
    timestamp = DOUBLE_ERROR;

  return(timestamp);

}




double getTime(int clockType) {

  clockid_t clockLinuxType = CLOCK_REALTIME;
  struct timeval myTime;
  struct timespec curTime;
  int rc;

  switch(clockType) {
    case 1:
	rc= gettimeofday (&myTime, (struct timezone *) NULL);
        if (rc==0) 
	  return (((((double) myTime.tv_sec) * 1000000.0) 
             + (double) myTime.tv_usec) / 1000000.0); 
        else{
         printf("getTime: Error on gettimeofday:%d, errno:%d (clockType:%d) \n",
           rc,errno,clockType);
          return(0.0);
        }

    break;

    case 2:
      clockLinuxType = CLOCK_REALTIME;
      //Use clock_gettime
      rc =clock_gettime(clockLinuxType, &curTime);
      if (rc==0) { 
        //return (1000000000 * (double)curTime.tv_sec + (double)curTime.tv_nsec);
        return ((double)curTime.tv_sec + ((double)curTime.tv_nsec)/1000000000);
      }
      else{
        printf("getTime: Error on clock_gettime:%d, errno:%d (clockType:%d) \n",
          rc,errno,clockType);
        return(0.0);
      }
    break;

    case 3:
      clockLinuxType = CLOCK_PROCESS_CPUTIME_ID;
      //Use clock_gettime
      rc =clock_gettime(clockLinuxType, &curTime);
      if (rc==0) { 
        //return (1000000000 * (double)curTime.tv_sec + (double)curTime.tv_nsec);
        return ((double)curTime.tv_sec + ((double)curTime.tv_nsec)/1000000000);
      }
      else{
        printf("getTime: Error on clock_gettime:%d, errno:%d (clockType:%d) \n",
          rc,errno,clockType);
        return(0.0);
      }
    break;

    default:
      printf("Error on clockType: %d \n",clockType);
      return(0.0);
  }

}

double getTime1()
{
	struct timeval curTime;
	(void) gettimeofday (&curTime, (struct timezone *) NULL);
	return (((((double) curTime.tv_sec) * 1000000.0) 
             + (double) curTime.tv_usec) / 1000000.0); 
}


int gettimeofday_benchmark()
{
  int i;
  struct timespec tv_start, tv_end;
  struct timeval tv_tmp;
  int count = 1 * 1000 * 1000 * 50;
  clockid_t clockid;
        
  int rv = clock_getcpuclockid(0, &clockid);

  if (rv) {
    perror("clock_getcpuclockid");
    return 1;
  }

  clock_gettime(clockid, &tv_start);

  for(i = 0; i < count; i++)
      gettimeofday(&tv_tmp, NULL);

  clock_gettime(clockid, &tv_end);

  long long diff = (long long)(tv_end.tv_sec - tv_start.tv_sec)*(1*1000*1000*1000);
  diff += (tv_end.tv_nsec - tv_start.tv_nsec);

  printf("%d cycles in %lld ns = %f ns/cycle\n", count, diff, (double)diff / (double)count);

  return 0;

}


int
delay(int64_t ns)
{
    struct timespec req, rem;

    req.tv_sec = 0;

    while (ns >= 1000000000L) {
        ns -= 1000000000L;
        req.tv_sec += 1;
    }

    req.tv_nsec = ns;

    while (nanosleep(&req, &rem) == -1)
        if (EINTR == errno)
            memcpy(&req, &rem, sizeof(rem));
        else
            return -1;
    return 0;
}


/*************************************************************
* Function: int convertTimespecToString(char *callersTimeString, struct timespec *myTS)
* 
* Summary: converts the caller's timestamp (a timespec) to
*          a human readable string which is placed in the callersTimeString buffer.
*
* Inputs:   
*   char *callersTimeString: reference to caller's buffer to hold the string
*   int maxSize:  size of caller's buffer
*   struct timespect *myTS:   caller's timestamp
*
* outputs: returns ERROR or number of characters in the string
*   One error is if the caller's buffer is  not large enough
*
*************************************************************/
int convertTimespecToString(char *callersTimeString,int maxSize, struct timespec *myTS)
{
  int rc = NOERROR;
  int dataSize = MAX_TMP_BUFFER;
  long int testTime;
  char *timeString=NULL;
  time_t timeInSeconds = 0;
  int sizeOfString=0;

  double callersTime = convertTS2D(myTS);
#ifdef TRACEME
  printf("convertTimespecToString: callers Time: %9.9f maxSize:%d \n",callersTime,maxSize);
#endif

  if (myTS == NULL)
   rc = ERROR;
  else {
   timeInSeconds = myTS->tv_sec;
   timeString  = malloc((size_t)MAX_LINE_SIZE); 
   bzero((void *)timeString, (size_t) MAX_LINE_SIZE);
   timeString =  ctime((const time_t *)&timeInSeconds);
   sizeOfString = strlen(timeString);
   if (timeString != NULL) {
#ifdef TRACEME
      printf("convertTimespecToString: size:%d %s \n",sizeOfString, timeString);
#endif
     if (sizeOfString <= maxSize) {
       // char *strcpy(char *dest, const char *src);
       // char *strncpy(char *dest, const char *src, size_t n);
       //strncpy(callersTimeString, timeString, (size_t) maxSize);
       strcpy(callersTimeString, timeString);
     } else 
       rc = ERROR;

   } else {
     rc = ERROR;
   }
  }

  if (rc == NOERROR) {
   rc = sizeOfString;
  }

#ifdef TRACEME
  if (rc == ERROR) {
    printf("convertTimespecToString: ERROR ??  size:%d %s \n",sizeOfString, timeString);
  } else {
    printf("convertTimespecToString: SUCCESS:  return size:%d %s \n",rc,  timeString);
  }
#endif

  return rc;
}


/***********************************************************
* Function:  double convertTS2D(struct timespec *t) 
*
* Explanation:  This converts the timestamp from 
*             a timespec into a double
*
* inputs: 
*  struct timespec *callersTime - the caller's ptr to the timestamp
*
* outputs:
*        returns the timestamp as a double in units of seconds with nsec precision
*
* notes: 
*
***********************************************************/
double convertTS2D(struct timespec *t) {
  return ( t->tv_sec + ( (double) t->tv_nsec)/1000000000 );
}


