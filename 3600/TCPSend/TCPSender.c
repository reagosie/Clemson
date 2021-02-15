/*********************************************************
* Module Name:  A simple TCP sender - sends forever as fast as the socket permits
*
* File Name:    TCPSender.c
*
* Params:   
*     ServerName : Name of the receiving host  
*     Service/port :port or service name
*     dataSize :   number of bytes to send.  0 means forever
*     chunkSize :  optional - specifies how much data is transferred 
*                           on each send socket call.
*     pipesize :   optional -  can specify the socket Tx buffer size
*
* Summary:
*        Loops forever, sending a chunkSize of data at a time.
*        Does not issue a timeout.
*        Two ways to end:  CNT-C  or the server stops, causing the send
*           to end in error.
*
* Last update: 4/8/2020
*
*********************************************************/
#include "common.h"
#include "utils.h"
#include "SocketHelper.h"


void finalStats();
void clientCNTCCode();
void CatchAlarm(int ignored);

//uncomment to see debug trace
//#define TRACEME 1

//Define the following variables globally so our async handlers can access them

char *server = NULL;                   /* IP address of server */
char *service = NULL;                   /*service name */
int sock = -1;                         /* Socket descriptor */
double  wallTime = 0.0;
double firstSendTime = 0.0;
double lastSendTime = 0.0;
double totalBytesSent=0.0;
uint32_t lastErrno=NOERROR;

uint32_t numberTOs=0;   //currently not used

void myUsage() 
{
  printf("TCPTx:  <server name>  <service> <dataSize>  <chunkSize>  <pipesize> \n");
  printf("   <server name> :server's name or address \n");
  printf("   <service> : port or service name \n");
  printf("   <dataSize> : total number bytes to send (0, default, means send forever) \n");  
  printf("   <chunkSize>  : amount of data to send each socket send (default 1000000) \n");
  printf("   <pipesize> :  Size in bytes the socket Tx and Rx buffers set  (default -1 which means use whatever is configured \n");
}



int main(int argc, char *argv[])
{
  int32_t rc = NOERROR;
  bool loopFlag = true;
  uint32_t dataSize = 0;
  uint32_t chunkSize = 1000000;
  int32_t pipeSize = -1;
  char *TxBuffer = NULL;
  int32_t numBytes = 0;
  int32_t modBytes  = 0;
  int32_t numberChunks=0;
  int32_t sendSize=0;
  bool loopForever = true;

  //For SetSockOptions
  int32_t sockOption = -1;
  int32_t optionValue  = -1;
  void *optionValuePtr = (void *)&optionValue;
  int32_t RxSocketBufSize = -1;
  int32_t RxSocketBufSize2 = -1;
  int32_t TxSocketBufSize = -1;
  int32_t TxSocketBufSize2 = -1;


  setVersion(VersionLevel);

  if (argc <= 2)    /* need at least server name and port */
  {
    myUsage();
    exit(EXIT_FAILURE);
  }

  wallTime = getCurTimeD();

  server = argv[1];             /* First arg: server IP address (dotted quad) */
  service = argv[2];

  if (argc >3 ) 
    dataSize  = atoi(argv[3]);

  if (argc > 4 )
    chunkSize  = atoi(argv[4]);

  if (argc >5)
     pipeSize = atoi(argv[5]);


  if (dataSize == 0) {
     loopForever = true;
     modBytes  = -1;
     numberChunks=-1;
  }
  else {
     loopForever = false;
     if (dataSize < chunkSize) {
       modBytes = dataSize;
       numberChunks=0;
     }
     else { 
       numberChunks = dataSize / chunkSize;
       modBytes = dataSize % chunkSize;
     }
  }
    
//#ifdef TRACEME
  printf("%s: server:%s  service:%s dataSize:%d  numberChunks:%d, modChunk:%d chunkSize:%d pipeSize:%d  \n",
      argv[0],server, service, dataSize, numberChunks, modBytes, chunkSize, pipeSize);
//#endif

 //Init memory for first send
  TxBuffer = malloc((size_t)chunkSize);
  if (TxBuffer == NULL) {
    printf("client: HARD ERROR malloc of Tx  %d bytes failed \n", chunkSize);
    exit(1);
  }
  memset(TxBuffer, 0, chunkSize);

  signal (SIGINT, clientCNTCCode);

  // Create a connected TCP socket
  sock = SetupTCPClientSocket(server, service);
  if (sock == ERROR ) {
    printf("TCPSender:  Failed SetupTCPClientSocket \n");
    rc = ERROR;
    exit(EXIT_FAILURE);
  }

  //Learn the current socket buffer sizes, and then set to 
  // the configured pipesize
  sockOption =  SO_RCVBUF;
  rc = GetSocketOption(sock, sockOption);
  if (rc != EXIT_FAILURE) { 
    optionValue = rc;
    rc = NOERROR;
    RxSocketBufSize = optionValue;
    sockOption =  SO_SNDBUF;
    rc = GetSocketOption(sock, sockOption);
    if (rc != EXIT_FAILURE) { 
      optionValue = rc;
      rc = NOERROR;
      TxSocketBufSize = optionValue;
//#ifdef TRACEME
      printf("%s: ORIGINAL  RCVBUF;%d,  SNDBUF;%d,  pipeSize:%d  \n",argv[0],RxSocketBufSize,TxSocketBufSize,pipeSize);
//#endif
      //Now set to the desired size
      sockOption =  SO_RCVBUF;
      optionValue = pipeSize;
      if (pipeSize != -1) {
        rc = SetSocketOption(sock, sockOption, optionValuePtr, sizeof(int32_t));
        if (rc != EXIT_FAILURE) { 
          RxSocketBufSize2 = GetSocketOption(sock, sockOption);
        } 
        else {
           printf("%s: WARNING: Error on setSocketOption 1 to pipeSize:%d, errno:%d  \n",argv[0],pipeSize,errno);
        } 
        sockOption =  SO_SNDBUF;
        optionValue = pipeSize;
        rc = SetSocketOption(sock, sockOption, optionValuePtr, sizeof(int32_t));
        if (rc != EXIT_FAILURE) { 
          TxSocketBufSize2 = GetSocketOption(sock, sockOption);
        } 
        else {
           printf("%s: WARNING: Error on setSocketOption 2 to pipeSize:%d, errno:%d  \n",argv[0],pipeSize,errno);
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


  uint32_t loopCount=0;
  sendSize = chunkSize;
  firstSendTime = getCurTimeD();
  while (loopFlag)
  {
    
    if (!loopForever) {

      if ((numberChunks == 0) && (modBytes==0)) {
#ifdef TRACEME
            printf("%s:(%u) Terminating loop dataSize:%d  totalBytesSent:%9.0f  (numberChunks:%d, modChunk:%d)\n",
      argv[0],loopCount, dataSize, totalBytesSent, numberChunks, modBytes);
#endif
           loopFlag = false;
           break;
      }

      if (numberChunks > 0){
          sendSize = chunkSize;
          numberChunks--;
      }
      else if ((numberChunks == 0) && (modBytes >0)) {
          sendSize = modBytes;
          modBytes=0;
      }
    }
#ifdef TRACEME
    if (loopForever)
      printf("%s:(%d) FOREVER: sendSize:%d \n", argv[0],loopCount, sendSize);
    else 
      printf("%s:(%d) NOTFOREVER:  sendSize:%d numberChunks=%d dataSize:%d totalBytesSent:%9.0f \n", 
            argv[0],loopCount, sendSize, numberChunks, dataSize,totalBytesSent);
#endif

    numBytes = send(sock, TxBuffer, sendSize,0 );
    lastErrno=errno;
    if (numBytes == ERROR ) {
        perror("TCPTx: error on send  \n");
        rc = ERROR;
        loopFlag = false;
        break;
    } else  if (numBytes > 0) 
    {    
      if (numBytes != sendSize)
           printf("TCPTx:  WARNING: only sent %d bytes of required %d bytes \n",
                    numBytes, sendSize);

      lastSendTime =  getCurTimeD();
      totalBytesSent += (double) numBytes;
    }

    loopCount++;
#ifdef TRACEME
     printf("%s(%d): Sent %d bytes  totalBytesSent:%f  \n", 
         argv[0], loopCount, numBytes, totalBytesSent);
#endif 

  }
  loopFlag = false;
  finalStats();
  exit(rc);
}


void finalStats() 
{
  double avgThroughput  = 0.0;

  double duration = 0.0;


  wallTime = getCurTimeD();
  duration = lastSendTime - firstSendTime;

  if  (duration > 0) {
    avgThroughput = totalBytesSent*8 / duration;
  }

  printf("TCPTx: finalStats \n");
  printf("wallTime  lastErrno \tduration \tavgThroughput (bps)  totalBytesSent\n");
  printf("%12.9f  %d  %6.9f %12.0f %12.0f  \n",
          wallTime, lastErrno, duration, avgThroughput, totalBytesSent);

  if (sock != -1){
    close(sock);
    sock = -1;
  }
  exit(EXIT_SUCCESS);
}

void clientCNTCCode() 
{
  finalStats();
}



// Handler for SIGALRM
void CatchAlarm(int ignored) {
  numberTOs++;
}





