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
* Last update: 4/22/2020
*
*********************************************************/

/* Reagan Leonard ********/
/* CPSC 3600 *************/
/* Exam 3, Q11 ***********/
/* 4-22-20 ***************/
/*************************/

//NOTE
/* I believe almost all the code that was needed for this problem (Q11) was able to be copied and pasted directly from
  UDPEchoV3/client.c which is what I did (with the exception of a few lines...). All code that I have added to the
  original TCPSender.c file can be found by scrolling through and finding what is in between long lines of /**** / comments
  I have noted the sections that were just copied. Otherwise the added code is mine. I have tried to comment any and all
  code that I have added to attempt to explain my though process for why to add this code and why in that specific place.
  I am unsure if this code file will work properly with y'all's modified Receiver file, but I believe I have the right
  idea about what to do and what to change. I also believe that if it doesn't not work properly, I could modify it to work
  if given more time. But this is what I have to turn in at the moment.
*/


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

/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//struct to define what a message header contains
//we will give each chunk a new message header
//that is how we will achieve the goal of "send an updated sequence number and send time stamp for each new chunk sent"
typedef struct {
  uint32_t sequenceNum;
  uint32_t timeSentSeconds;
  uint32_t timeSentNanoSeconds;
} messageHeaderDefault;
/**********************************************************************************************************************/

/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//create 2 header pointers, one for Tx (sending) and one for Rx (receiving), and set them to NULL
messageHeaderDefault *TxHeaderPtr=NULL;
messageHeaderDefault *RxHeaderPtr=NULL;
//create 2 char buffers, one for Tx and one for Rx and set them to NULL
//these are the buffers that will store the message we are sending
char *TxBuffer = NULL;
char *RxBuffer = NULL;
/**********************************************************************************************************************/

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
/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//not entirely sure if this is necessary here but I don't think it could harm the function of the program
//it was in client.c so it might be necessary here
  startTime = wallTime;
/**********************************************************************************************************************/

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

/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//not entirely sure if this is necessary here but I don't think it could harm the function of the program
//it was in client.c so it might be necessary here
  sequenceNumber++;
/**********************************************************************************************************************/

 //Init memory for first send
  TxBuffer = malloc((size_t)chunkSize);
  if (TxBuffer == NULL) {
    printf("client: HARD ERROR malloc of Tx  %d bytes failed \n", chunkSize);
    exit(1);
  }
  memset(TxBuffer, 0, chunkSize);

/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//as Dr. Martin's comment below says, we will use this TxIntPtr as the instrument to pack the TxBuffer
  //This pointer is used when packing the header into the network buffer
  TxIntPtr  = (uint32_t *) TxBuffer;

  messageHeaderDefault TxHeader;
  TxHeaderPtr=&TxHeader;
/**********************************************************************************************************************/

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

/*******************************
********************************
ACTUAL SEND LOOP
********************************
********************************/
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
/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//update the TxHeader after each loop iteration (each chunk sent)
//do this by getting a new timestamp and incrementing the sequence number
    lastMsgTxWallTime = getCurTime(&msgTxTime);
    wallTime = lastMsgTxWallTime;
    //Update the TxHeader
    TxHeaderPtr->sequenceNum = sequenceNumber++;
    TxHeaderPtr->timeSentSeconds = msgTxTime.tv_sec;
    TxHeaderPtr->timeSentNanoSeconds = msgTxTime.tv_nsec;

//then pack this new information into the actual buffer (TxBuffer) using our TxIntPtr
    //pack the header into the network buffer
    TxIntPtr  = (uint32_t *) TxBuffer;
    *TxIntPtr++  = htonl(TxHeaderPtr->sequenceNum);
    *TxIntPtr++  = htonl(TxHeaderPtr->timeSentSeconds);
    *TxIntPtr++  = htonl(TxHeaderPtr->timeSentNanoSeconds);
/**********************************************************************************************************************/

#ifdef TRACEME
     printf("%s(%d): Sent %d bytes  totalBytesSent:%f  \n",
         argv[0], loopCount, numBytes, totalBytesSent);
#endif

  }
  loopFlag = false;
  finalStats();
  exit(rc);
}

/**********************************************************************************************************************/
//copied from UDPEchoV3/client.c

//at this point we have exited the sending loop and the main() function
//we now need to set the sequence number to all 0s to signal the closing of the session
//also get a new timestamp and put this into TxHeaderPtr

//Update the TxHeader
TxHeaderPtr->sequenceNum = 0xffffffff;
TxHeaderPtr->timeSentSeconds = msgTxTime.tv_sec;
TxHeaderPtr->timeSentNanoSeconds = msgTxTime.tv_nsec;

//then we put this information into our TxBuffer below

//pack the header into the network buffer
TxIntPtr  = (uint32_t *) TxBuffer;
*TxIntPtr++  = htonl(TxHeaderPtr->sequenceNum);
*TxIntPtr++  = htonl(TxHeaderPtr->timeSentSeconds);
*TxIntPtr++  = htonl(TxHeaderPtr->timeSentNanoSeconds);
/**********************************************************************************************************************/

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
