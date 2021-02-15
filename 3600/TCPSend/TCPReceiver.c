/*********************************************************
* Module Name:  A simple TCP Receiver - it sinks all data.
*               It ends in one of two ways:  The client terminates.
*                which will likely cause one of several possible errors unblocking a recv.
*               A CNT-C is issued to the program.
*               In any case, when the main loop breaks, the observed avg throughput is displayed
*                
*               This is an iterative server meaning it does not fork a
*               child process for each client request. Instead, a single process
*               handles a client to completion and then proceeds to 
*               process the next client waiting in the Accept queue.
*
* File Name:    TCPReceiver.c
*
* Program Name:  TCPRx
*
* params: 
*    char *service :  port or service
*    int32_t chunkSize : The amount of data read each recv. Default is 1000000 bytes
*    int32_t pipeSize :  The desired size of the socket Tx and Rx buffers. Default
*                        is -1 which will use whatever the default setting is.        
*
*
* Last update: 4/7/2020
*
*********************************************************/
#include "common.h"
#include "utils.h"
#include "SocketHelper.h"


void finalStats();
void CNTCCode();
void CatchAlarm(int32_t ignored);

//uncomment to see debug trace
//#define TRACEME 1

//Define the following variables globally so our async handlers can access them

int32_t sock = -1;                         /* Socket descriptor */
int32_t sockClient = -1;                         /* Socket descriptor */

double totalByteCount = 0.0;

double  wallTime = 0.0;
double firstRxTime = 0.0;
double lastRxTime = 0.0;
uint32_t lastErrno=NOERROR;


/* Global so accessable by SIGCHLD signal handler */
uint32_t childProcCount = 0;   /* Number of child processes */



void myUsage() 
{
  printf("TCPRx: <service>  <chunkSize>  <pipesize> \n");
}



int main(int32_t argc, char *argv[])
{

  int32_t rc = EXIT_SUCCESS;
  char *service = NULL;  //sets port number
  int32_t chunkSize  = 1000000;
  int32_t pipeSize = -1;
  bool  loopFlag = true;
  bool  outerLoopFlag = true;
  int32_t  totalBytesRcvd = 0;
  int32_t  bytesRcvd = 0;
  char *RxBuffer = NULL;

  //For SetSockOptions
  int32_t sockOption = -1;
  int32_t optionValue  = -1;
  void *optionValuePtr = (void *)&optionValue;
  int32_t RxSocketBufSize = -1;
  int32_t RxSocketBufSize2 = -1;
  int32_t TxSocketBufSize = -1;
  int32_t TxSocketBufSize2 = -1;

  setVersion(VersionLevel);


  if (argc <= 1)    /* need at port */
  {
    myUsage();
    exit(1);
  }

  wallTime = getCurTimeD();



  service = argv[1];

  if (argc >2 )
    chunkSize  = atoi(argv[2]);

  if (argc >3)
     pipeSize = atoi(argv[3]);

  RxBuffer = (char *)malloc((size_t) chunkSize);
  if (RxBuffer == NULL) {
    printf("server: HARD ERROR malloc of Tx  %d bytes failed \n", chunkSize);
    exit(1);
  }
  memset(RxBuffer, 0, chunkSize);

  signal (SIGINT, CNTCCode);



//#ifdef TRACEME
  printf("%s(Version:%s): pid:%d  %d args service:%s csize:%d, pSize:%d \n", 
           argv[0], getVersion(),(int32_t) getpid(), argc, service,  chunkSize, pipeSize);
//#endif


  while (outerLoopFlag) 
  {
    loopFlag = true;
     // Create a connected TCP socket
    sock =  SetupTCPServerSocket(service);
    if (sock < 0) {
      printf("%s: HARD error on SetupTCPServerSocket: rc:%d,  service:%s,  errno:%d \n",
         argv[0],sock, service, errno);
      rc = EXIT_FAILURE;         
      exit(rc);
    }



#ifdef TRACEME
    printf("%s: Blocking in an Accept waiting for a client \n", 
           argv[0]);
#endif
    //Returns ERROR or valid sock descriptor
    rc = AcceptTCPConnection(sock);

    if (rc == ERROR ) {
      perror("TCPRx: HARD ERROR on AcceptTCPCx: ");
      exit(EXIT_FAILURE);
    } else {
      sockClient = rc;
#ifdef TRACEME
    printf("%s(Version:%s): AcceptTCPCx succeeded \n", 
           argv[0], getVersion());
#endif

    }


  //Learn the current socket buffer sizes, and then set to 
  // the configured pipesize
  sockOption =  SO_RCVBUF;
  rc = GetSocketOption(sockClient, sockOption);
  if (rc != EXIT_FAILURE) { 
    optionValue = rc;
    rc = NOERROR;
    RxSocketBufSize = optionValue;
    sockOption =  SO_SNDBUF;
    rc = GetSocketOption(sockClient, sockOption);
    if (rc != EXIT_FAILURE) { 
      optionValue = rc;
      rc = NOERROR;
      TxSocketBufSize = optionValue;
//#ifdef TRACEME
      printf("%s: ORIGINAL  RCVBUF;%d,  SNDBUF;%d  \n",argv[0],RxSocketBufSize,TxSocketBufSize);
//#endif
      //Now set to the desired size
      sockOption =  SO_RCVBUF;
      optionValue = pipeSize;
      if (pipeSize != -1) {
        rc = SetSocketOption(sockClient, sockOption, optionValuePtr, sizeof(int32_t));
        if (rc != EXIT_FAILURE) { 
          RxSocketBufSize2 = GetSocketOption(sockClient, sockOption);
        } 
        sockOption =  SO_SNDBUF;
        optionValue = pipeSize;
        rc = SetSocketOption(sockClient, sockOption, optionValuePtr, sizeof(int32_t));
        if (rc != EXIT_FAILURE) { 
          TxSocketBufSize2 = GetSocketOption(sockClient, sockOption);
        } 
      }
    } else 
      printf("%s: WARNING: Error on GetSocketOption 1, errno:%d  \n",argv[0],errno);
  } else
    printf("%s: WARNING: Error on GetSocketOption 2, errno:%d  \n", argv[0],errno);

//#ifdef TRACEME 
  if ((RxSocketBufSize2 != ERROR) && (TxSocketBufSize2 != ERROR))
    printf("%s:  SetSocketOption succeeded: Original RxSocketBufSize:%d and TxSocketBufSize:%d,  Updated RxSocketBufSize:%d and  TxSocketBufSize:%d  \n", 
          argv[0], RxSocketBufSize,  TxSocketBufSize, RxSocketBufSize2,TxSocketBufSize2);
  else 
    printf("%s:  SetSocketOption Failed?? original RxSocketBufSize:%d and TxSocketBufSize:%d \n",
          argv[0], RxSocketBufSize,  TxSocketBufSize);
//#endif


  //Inner loop -  service a client to completion
  firstRxTime = getCurTimeD();
  while (loopFlag)
  {
    totalBytesRcvd = 0;
    while (totalBytesRcvd < chunkSize) 
    {
        bytesRcvd = recv(sockClient, RxBuffer, chunkSize, 0);
        lastErrno=errno;
        if (bytesRcvd == ERROR) {
           loopFlag = false;
           rc = ERROR;
           perror("TCPRx: Error returned on recv ");
           break;
        } else if (bytesRcvd == 0) {
           loopFlag = false;
           //rc = ERROR;
           //printf("TCPRx: HARD ERROR, rxed 0 bytes \n");
           break;
        } else {
          lastRxTime =  getCurTimeD();
          totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
#ifdef TRACEME
          printf("%s: bytesRcvd:%d  \n", argv[0], bytesRcvd);
#endif
       }
    }
    totalByteCount += (double)totalBytesRcvd;

#ifdef TRACEME
    printf("%s: Received: %d bytes, totalByteCount:%12.0f\n", argv[0], totalBytesRcvd,totalByteCount);
#endif

  } // end of while loop servicing one client

  loopFlag = false;
  finalStats();
  //TODO: Call routine to reset stats
  firstRxTime= 0.0;
  lastRxTime= 0.0;
  totalByteCount=0;
  //Close client socket AND main socket....it will crreate a new socket
  if (sockClient != ERROR) {
    close(sockClient);
    sockClient = ERROR;
  }
  if (sock != ERROR) {
    close(sock);
    sock = ERROR;
  }


  }  //outerloop main loop that services new clients 

  //If we get here, we need to finish up with this client....

  outerLoopFlag = false;
  loopFlag = false;
  finalStats();
  exit(rc);
}

void finalStats() 
{
  double duration = 0.0;
  double avgThroughput=0.0;

  wallTime = getCurTimeD();
  duration = lastRxTime - firstRxTime;

  if (duration > 0 ) {
    avgThroughput = totalByteCount * 8 / duration;
  } 



  if (totalByteCount == 0) {
     printf("TCPRx(%12.9f): %d  %6.6f  No packet arrivals !! \n",
          wallTime, lastErrno,duration);
  } else {
     printf("TCPRx: finalStats \n");
     printf("wallTime  lastErrno \tduration \tavgThroughput (bps)  totalBytesReceived\n");
     printf("%12.9f  %d  %6.6f %12.0f %12.0f \n",
          wallTime, lastErrno, duration, avgThroughput, totalByteCount);
  }

  if (sock != -1){
    close(sock);
    sock = -1;
  }
}

void CNTCCode() 
{
  finalStats();
}





