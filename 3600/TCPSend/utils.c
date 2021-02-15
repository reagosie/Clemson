/*********************************************************
* Module Name:  Tools common routines 
*
* File Name:    utils.c
*
*
*********************************************************/
#include "utils.h"

//#define TRACEME 0


//Holds the  version level text,  init client or server must call setVersion
char Version[MAX_LINE_SIZE] = "0.0";

int daemon_proc;   /* set nonzero by daemon_init() */

/***********************************************************
* Function: void setVersion(double versionLevel) {
*
* Explanation:  This must be called by the main program
*               to set the Version. 
* inputs:   The version number 
*
* outputs:
*
* notes: 
*
**************************************************/
void setVersion(double versionLevel) {

char *versionPtr = Version;
  sprintf(versionPtr,"Version %1.2f",versionLevel);
}

/***********************************************************
* Function: char *getVersion() {
*
* Explanation: Returns a ptr to the version string.
* inputs:    
*
* outputs: Returns a ptr to the global string
*
* notes: 
*
**************************************************/
char *getVersion() {
   return (Version);
}


void DieWithUserMessage(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

void DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

//Returns timeval in seconds with usecond precision
double convertTimeval(struct timeval *t) 
{
  return ( t->tv_sec + ( (double) t->tv_usec)/1000000 );
}


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


/***********************************************************
* Function: int udelay(int32_t delayUseconds)
*
* Explanation:  This delays the specified number of microseconds
*
* inputs: 
*    int32_t  delayUseconds : number of microseconds to delay 
*
* outputs:
*        returns FAILURE or SUCCESS
*
* notes: 
*
**************************************************/
int udelay(int32_t delayUseconds)
{
  int rc = SUCCESS;

  int64_t ns = (int64_t) (delayUseconds * 1000); 

  rc = nanodelay(ns);
  return rc;
}


/***********************************************************
* Function: int nanodelay(int64_t ns)
*
* Explanation:  This delays the specified number of nanoseconds
*
* inputs: 
*    int64_t ns : number of ns to delay 
*
* outputs:
*        returns FAILURE or SUCCESS
*
* notes: 
*
***********************************************************/
int nanodelay(int64_t ns)
{
struct timespec req, rem;
int rc = SUCCESS;

  req.tv_sec = 0;

  while (ns >= 1000000000L) {
        ns -= 1000000000L;
        req.tv_sec += 1;
  }

  req.tv_nsec = ns;

  while (nanosleep(&req, &rem) == -1) {
        if (EINTR == errno)
            memcpy(&req, &rem, sizeof(rem));
        else {
          rc = FAILURE;
          printf("delay: nanosleep return in error, rem.t_sec:%d t_nsec:%d,  errno: %d \n",(int) rem.tv_sec,(int) rem.tv_nsec,errno);
          // return -1;
        }
  }

  return  rc;
}

/***********************************************************
* Function: double timestamp() 
*
* Explanation:  This returns the current tiem
*               as a double. The value is therefore the
*               Unix time - the number of seconds.microseconds
*               since the beginning of the Universe (according to Unix)
*
* inputs: 
*
* outputs:
*        returns the timestamp.
*
* notes: 
*
***********************************************************/
double timestamp() 
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) { 
    printf("utils:timestampt: gettimeofday failed, errno: %d \n",errno); 
  }
  return ((double)tv.tv_sec + ((double)tv.tv_usec / 1000000));
}

/***********************************************************
* Function: int getTime(struct timeval *callersTime);
*
* Explanation:  This returns the current time
*               using the standard timeval format.
*
* inputs: 
*  struct timeval *callersTime - the caller's ptr to hold the time
*
* outputs:
*        returns SUCCESS or ERROR.
*
* notes: 
*
***********************************************************/
int getTime(struct timeval *callersTime)
{
int rc = SUCCESS;

  if (gettimeofday(callersTime, NULL) < 0) { 
     rc = ERROR;
     printf("utils:timestampt: gettimeofday failed, errno: %d \n",errno); 
  } 
  return rc;
}

/*************************************************************
* Function: int myDelay(double delayTime)
* 
* Summary: Delays the specified amount of time 
*
* Inputs:   double delayTime:  the time to delay in seconds
*
* outputs:  
*   returns  ERROR or SUCCESS
*
* Set CHECK_SLEEP_TIME to measure actual time
*
*************************************************************/
int myDelay(double delayTime)
{
  int rc = SUCCESS;

  int64_t ns = (int64_t) (delayTime * 1000000000.0); 
//  printf("myDelay:  delayTime:%f   %ld \n",delayTime, ns);

  rc = nanodelay(ns);
  return rc;
}


/*************************************************************
* Function: int is_bigendian()
* 
* Summary: returns TRUE if big endian else FALSE 
*
* Inputs:
* outputs:  
*   returns TRUE or FALSE 
* notes: 
*************************************************************/
int is_bigendian()
{
  int rc = FALSE;
unsigned int x = 0x12345678;
//  uint32_t x = 0x12345678;
  char *ptr = (char *) &x;

  //Test if this machine is big or little endian.
  if (ptr[0] == 0x01) {
    rc = TRUE;
  } else if (ptr[0] == 0x78) {
    rc = FALSE;
  } else {
    printf("is_bigendian: HARD ERROR ??   %0xX ??\n",ptr[0]);
  }
  return rc;
}


/*************************************************************
* Function: 
*   int packBufferWithInt(char *bufPtr, unsigned int intValue)
* 
* Summary: copies the intValue to the buffer in network byte order
*          Returns SUCCESS or ERROR 
*
* Inputs:
*   char *bufPtr
*   unsigned int intValue
*
* outputs:  
*   returns TRUE or FALSE 
* notes: 
*************************************************************/
int packBufferWithInt(char *bufPtr, unsigned int intValue)
{
  int rc = SUCCESS;
  unsigned int mySize = sizeof(intValue);
  unsigned int *myIntPtr = (unsigned int *)bufPtr;
  unsigned char *myCharPtrSrc = (unsigned char *)&mySize;
  unsigned char *myCharPtrDst = (unsigned char *)bufPtr;


  printf("packBufferWithInt: value: %d  size:%d \n",intValue, mySize);

  if(!is_bigendian()){
    //If this host is not bigendian 
    //swap octets
    myCharPtrDst[0]=myCharPtrSrc[3];
    myCharPtrDst[1]=myCharPtrSrc[2];
    myCharPtrDst[2]=myCharPtrSrc[1];
    myCharPtrDst[3]=myCharPtrSrc[0];
    
    printf("packBufferWithInt: Host little endian intValue:%0xX   packed:%0xX\n",intValue, *myIntPtr);
  } else {
    //Else this host is bigendian
    *myIntPtr = intValue;  
    printf("packBufferWithInt: Host big endian intValue:%d  packed:%d\n",intValue, *myIntPtr);
  }

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


//Darwin and BSD include this ....
#ifdef LINUX 

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
uint64_t htonll(uint64_t InAddr) 
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

#endif

static ssize_t my_read(int fd, char *ptr)
{
static int	read_cnt = 0;
static char	*read_ptr;
static char	read_buf[MAX_TMP_BUFFER];

  if (read_cnt <= 0) {
again:
    if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) 
    {
      if (errno == EINTR)
        goto again;
      return(-1);
    } else if (read_cnt == 0)
      return(0);

    read_ptr = read_buf;
  }

  read_cnt--;
  *ptr = *read_ptr++;
  return(1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
int n, rc;
char c, *ptr;

ptr = vptr;
  for (n = 1; n < maxlen; n++) {
    if ( (rc = my_read(fd, &c)) == 1) 
    {
      *ptr++ = c;
      if (c == '\n')
        break;	/* newline is stored, like fgets() */
    } else if (rc == 0) 
    {
      if (n == 1)
         return(0);	/* EOF, no data read */
      else
        break;		/* EOF, some data was read */
    } else
      return(-1);		/* error, errno set by read() */
  }

  *ptr = 0;	/* null terminate like fgets() */
  return(n);
}
/* end readline */

ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
ssize_t	n;

  if ( (n = readline(fd, ptr, maxlen)) < 0) {
    printf("Readline:  readline error\n");
    exit(EXIT_FAILURE);
  }
  return(n);
}

void err_sys(const char *fmt, ...)
{
  exit(1);
}



void sig_chld(int signo)
{
pid_t	pid;
int stat;

  printf("sigchldwaitpid(%d): Entered\n", (int) getpid());
  while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0) {
  }
  printf("sigchldwaitpid(%d): Exiting with pid:%d and stat:%d \n", (int) getpid(),pid, stat);
  return;
}

/***********************************************************
* Function: int   RxData(unsigned int maxLength, int readfd, int chunkSize);
*
* Explanation:  This routine reads up to maxLength
*               amount of data from the read descriptor
*               It returns the amount received or  EXIT_FAILURE
*               The data is just thrown away....
*
* inputs:  
*     uint32_t maxLength : max amount to read, if -1 read forever
*     int readfd :
*     int chunkSize :  
*
* outputs: returns  EXIT_FAILURE or the amount received
*           which could be 0 if that was the size.
*
*********************************************************/
int  RxData(unsigned int maxLength, int readfd, int chunkSize)
{
int rc=EXIT_FAILURE;
//tracks total data received
unsigned int  readCount=0;
char *RxBuffer=NULL;

  RxBuffer = (char*) malloc ( sizeof(char) * chunkSize);
  if (RxBuffer == NULL) {
    printf("utils:RxData: Malloc 1 error.....errno:%d \n", errno);
    return EXIT_FAILURE;
  }

  printf("RxData: rx up to %d bytes over descriptor :%d, chunkSize;%d \n",
          maxLength,readfd,chunkSize);

  double startTime = -1;
//  rc = read(readfd, RxBuffer, 4);
//  if (rc < 0) {
 //     printf("utils:RxData: HARD ERROR read error.....errno:%d \n", errno);
 //     return rc; 
 // } else 
 //   printf("utils:RxData: Rxed sync bytes... : %d arrived  \n",rc);

  startTime =  timestamp(); 
  while (readCount < maxLength) {
    rc = read(readfd, RxBuffer, chunkSize);
    if (rc < 0) {
      printf("utils:RxData: read error.....errno:%d \n", errno);
      break;
    } else { 
      readCount +=rc;
    }
  }
  double totalTime=  timestamp() - startTime;

  if (readCount != maxLength){
    printf("utils:RxData: WARNING: Partial data: expected:%d, received: %d \n",
           maxLength,readCount);
  }

  if (RxBuffer != NULL) 
    free(RxBuffer);

  double AvgThroughput=0;
  double AvgThroughput2=0;
  if (readCount > 0) { 
    AvgThroughput =  ((double)readCount * 8) / totalTime;
    AvgThroughput =  AvgThroughput / 1000000000 ;
    AvgThroughput2  =  AvgThroughput / 1000000 ;
    printf("RxData(%d): totalTime:%2.6f totalRxed:%d (chunkSize:%d) Throughput:%4.3f Gbps (%6.3f Mbps) \n", 
        (int) getpid(),totalTime,readCount,chunkSize,AvgThroughput,AvgThroughput2);
    return readCount;
  }
  else {
    printf("RxData(%f): FAILED ??   rc:%d,  totalRxed:%d, errno:%d \n", 
            totalTime, rc,readCount,errno);
    return EXIT_FAILURE;
  }
}
 

/***********************************************************
* Function: int  sendData(unsigned int  transferSize, int writefd, int chunkSize);
*
* Explanation:  This routine is called to send transferSize 
*               amount of data over the writefd
*               It returns the amount sent or EXIT_FAILURE
*
* inputs:   read and write file descriptors 
*     unsigned int : transferSize
*     int writefd :
*     int chunkSize :  amount of data sent each write
*
* outputs: returns  EXIT_FAILURE or the amount sent...
*           which could be 0 if that was the size.
*          It does not assume a failure if amount send < len 
*
*********************************************************/
int  sendData(unsigned int transferSize, int writefd, int chunkSize)
{
int rc=EXIT_FAILURE;
unsigned int totalSent = 0;
unsigned int chunkSendCount = 0;
unsigned int loopCount = 0;
char *bufPtr = NULL;
int sendCount = transferSize;

  printf("sendData:  sending %d bytes over descriptor :%d, chunkSize;%d \n",
          transferSize,writefd,chunkSize);

  bufPtr = (char*) malloc ( sizeof(char) * chunkSize);
  if (bufPtr == NULL) {
      printf("sendData: Malloc 1 error.....errno:%d \n", errno);
      return EXIT_FAILURE;
  }
 
  //sendCount: tracks how much is left to send
  while (sendCount > 0) {
    if (sendCount < chunkSize)
       chunkSendCount = sendCount;
    else
       chunkSendCount  = chunkSize;

      //chunkSendCount: tracks how much of current CHUNK  is left to send
#ifdef TRACEME 
      printf("utils:sendData: write chunk of size %d, total sent: %d, loopCount:%d \n",
            chunkSendCount,sendCount,loopCount+1);
#endif
    //inner loop to send up  to a chunkSize
    while (chunkSendCount > 0) {
        loopCount++;
        rc = write(writefd, bufPtr, chunkSendCount);
        if (rc < 0) {
          printf("sendData:  write error,  chunkSendCount:%d, totalSent:%d  errno:%d \n",
            chunkSendCount,totalSent, errno);
          rc = EXIT_FAILURE;
          break;
        }
        totalSent +=rc;
        chunkSendCount -= rc;
        sendCount -= rc;
    }
    if (rc < 0) {
      printf("sendData:  write error,  chunkSendCount:%d, totalSent:%d  errno:%d \n",
          chunkSendCount,totalSent, errno);
      rc = EXIT_FAILURE;
      break;
    }
    if (chunkSendCount != 0) {
      printf("sendData:  WARNING: chunkSendCount not 0 (%d), totalSent:%d rc:%d \n",
          chunkSendCount,totalSent, rc);
    }
  }

  if (bufPtr != NULL) 
    free(bufPtr);

#ifdef TRACEME 
  printf("utils:SendData: FINISHED:  rc:%d  totalSent %d, \n", rc, totalSent);
#endif

  if (rc == EXIT_SUCCESS)
    return totalSent;
  else
    return rc;

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




double getTimeByType(int clockType) {

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




