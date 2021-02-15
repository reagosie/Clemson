/*********************************************************
* Module Name: simple UDP Echo server 
*
* File Name:    server.c	
*
* Summary:
*  This file contains the client portion of a client/server
*    UDP-based performance tool.
*  
* Usage:
*     server <service> 
*     debugLevel :  0: prints to stdout only final stats
*                   1: prints to stdout each iteration sample AND final stats
*
* Last updated: 3/6/2020
*
*********************************************************/
#include "UDPEcho.h"
#include "utils.h"
#include "AddressUtility.h"
#include "session.h"

void finalStats();
void CatchAlarm(int ignored);
void CNTCCode();

int sock = -1;                         /* Socket descriptor */
int bStop = 1;;
FILE *newFile = NULL;
double startTime = 0.0;
double endTime = 0.0;
double  wallTime = 0.0;
uint32_t RxErrorCount = 0;
uint32_t TxErrorCount = 0;
uint32_t debugLevel = 1;
double   OWDSum = 0;
double numberFailedSessionLookups = 0;
uint32_t numberSessions=0;



void usage();

//uncomment to see debug output
//#define TRACEME 1

void usage() 
{
  printf("UPDEchoServerV3: <service/port> <debugLevel>\n");
  printf("UPDEchoServerV3: debugLevel: 0: only final stats shown, 1:iteration results shown\n");
  printf("UPDEchoServerV3: defaults:   port 5000 debuLevel 1 \n");

}

int main(int argc, char *argv[]) 
{

  char *buffer  = NULL;
  messageHeaderDefault msgHeader;
  messageHeaderDefault *msgHeaderPtr=&msgHeader;
  uint32_t *myBufferIntPtr  = NULL;
  char *service = "5000";
  double  OWDsample = 0.0;
  session *currentSession = NULL;
  int  numberClientSessionCloses=0;

  int sock = -1;
  struct sockaddr_storage clntAddr; // Client address
  struct sockaddr_in *clntAddrPtr = (struct sockaddr_in *) &clntAddr;

  // Set Length of client address structure (in-out parameter)
  socklen_t clntAddrLen = sizeof(clntAddr);

  double thisInterArrivalTime = -1.0;
  double thisJitter = -1.0;
  int tmpY = 0;
  bool loopFlag = true;
  bool wrapSeqNumber=false;
  ssize_t numBytesRcvd = 0;
  double  actualSeqNumber=0.0;
  ssize_t numBytesSent = 0;

#ifdef TRACEME 
  printf("UDPEchoServerV3: Entered with %d args \n", argc);
#endif

  if (argc == 2 ) // Test for correct number of arguments
  {
    char *ch = argv[1];
    if  ( (ch[0] == 'h') || (ch[0] == '?')) 
    {
      usage(); 
      exit(EXIT_SUCCESS);
    }
    else 
      service = argv[1]; // First arg:  local port/service
  }

  if (argc >2 ) //Get the second ard
    debugLevel = atoi(argv[2]); // Second arg:  debugLevel: 0 or 1

  // Construct the server address structure
  struct addrinfo addrCriteria;                   // Criteria for address
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram socket
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP socket

  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

  // Display returned addresses
  struct addrinfo *addr = NULL;
  for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
    PrintSocketAddress(addr->ai_addr, stdout);
    fputc('\n', stdout);
  }

  //Init memory for first send
  buffer = malloc((size_t)MAX_DATA_BUFFER);
  if (buffer == NULL) {
    printf("server: HARD ERROR malloc of  %d bytes failed \n", MAX_DATA_BUFFER);
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, MAX_DATA_BUFFER);

  signal (SIGINT, CNTCCode);

  // Create socket for incoming connections
  sock = socket(servAddr->ai_family, servAddr->ai_socktype,
      servAddr->ai_protocol);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Bind to the local address
  if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
    DieWithSystemMessage("bind() failed");

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  wallTime = getCurTimeD();
  startTime = wallTime;
  while (loopFlag)
  { 
    // Run forever
    // Set Length of client address structure (in-out parameter)
    clntAddrLen = sizeof(clntAddr);
    wrapSeqNumber=false;

    // Block until receive message from a client
    // Size of received message
    numBytesRcvd = recvfrom(sock, buffer, MAX_DATA_BUFFER, 0,
        (struct sockaddr *)clntAddrPtr, &clntAddrLen);

    if (numBytesRcvd < 0){
      RxErrorCount++;
      perror("server: Error on recvfrom ");
      continue;
    } else if (numBytesRcvd < MESSAGEMIN) {
      RxErrorCount++;
      printf("server: Error on recvfrom, received (%d) less than MIN (%d) \n ", numBytesRcvd,MESSAGEMIN);
      continue;
    } else 
    { 

      wallTime = getCurTimeD();
      myBufferIntPtr  = (uint32_t *)buffer;
      //unpack to fill in the rx header info
      msgHeaderPtr->sequenceNum = ntohl(*myBufferIntPtr++);
      msgHeaderPtr->timeSentSeconds = ntohl(*myBufferIntPtr++);
      msgHeaderPtr->timeSentNanoSeconds = ntohl(*myBufferIntPtr++);

      if (msgHeaderPtr->sequenceNum == (UINT_MAX-1))
      {
        //sequenceNum wraps after this 
        wrapSeqNumber=true;
      }
      if (msgHeaderPtr->sequenceNum == UINT_MAX)
      {
        //If UINT_MAX, this is the signal to stop
        //session has completed ....
 // ... "close" the session
//        sendFlag = FALSE;
        numberClientSessionCloses++;
        //currentSession =  getActive(clntAddr.sin_addr, clntAddr.sin_port);
        //if (currentSession == NULL) {
        //  numberFailedSessionLookups++;
        //  printf("perfServer:(%f) ERROR: Null returned on getActive ,  number of sessions: %d \n",
        //    wallTime, getNumberSessions());
        //  exit(EXIT_FAILURE);
        // }
        //For now, we put all sessions on the active list
        //updateSessionDuration(currentSession);
        //moves session from the active list to archived list
        // rc = removeActive(clntAddr.sin_addr, cl

#ifdef TRACEME
        printf("UDPEchoServer(%f): Session ended client: %s \n",
               wallTime, inet_ntoa(clntAddrPtr->sin_addr));
#endif
   
        //will not return
        //finalStats();
      } else
      {

        currentSession = getActive(clntAddrPtr->sin_addr, clntAddrPtr->sin_port);
        if (currentSession == NULL) {
          numberFailedSessionLookups++;
          printf("UDPEchoServerV3:(%f) ERROR: Null on getActive,number of sessions: %d \n",
                 wallTime, getNumberSessions());
          exit(EXIT_FAILURE);
        }

        if (!currentSession->isActive ){
          printf("UDPEchoServerV3:(%f) ERROR: currentSession is archived ?? \n",
              wallTime);
          exit(EXIT_FAILURE);
        }

        actualSeqNumber= (msgHeaderPtr->sequenceNum + 
                           (currentSession->sequenceNumberWrap*(UINT_MAX)));

        OWDsample = wallTime - (msgHeaderPtr->timeSentSeconds + 
                              (msgHeaderPtr->timeSentNanoSeconds / 1000000000));

        OWDSum  += OWDsample;
        if (actualSeqNumber > currentSession->largestSeqRecv)
          currentSession->largestSeqRecv = actualSeqNumber;

          currentSession->lastArrivalTimeD = wallTime;
          currentSession->bytesReceived += numBytesRcvd;
          currentSession->messagesReceived++;

        if (debugLevel == 1 ) {
          printf("%f %d %d %f %f %f %3.9f \n", wallTime, numBytesRcvd,
             msgHeaderPtr->sequenceNum, actualSeqNumber,
             currentSession->messagesReceived,
             currentSession->largestSeqRecv,
             OWDsample);
        }

#ifdef TRACEME 
        printf("server: Rx %d bytes from ", (int) numBytesRcvd);
        fputs(" client ", stdout);
        struct addrinfo *addr = NULL;
        PrintSocketAddress((struct sockaddr *)clntAddrPtr, stdout);
        fputc('\n', stdout);
#endif

        // Send received datagram back to the client
        numBytesSent = sendto(sock, buffer, numBytesRcvd, 0,
        (struct sockaddr *)clntAddrPtr, sizeof(clntAddr));
        if (numBytesSent < 0) {
          TxErrorCount++;
          perror("server: Error on sendto ");
          continue;
        }
        else if (numBytesSent != numBytesRcvd) {
          TxErrorCount++;
          printf("server: Error on sendto, only sent %d rather than %d ",numBytesSent,numBytesRcvd);
          continue;
        }

        if (currentSession->firstArrivalTimeD == -1) {
          currentSession->sessionID = numberSessions++;
          currentSession->firstArrivalTimeD = wallTime;
          currentSession->delayChange = 0;
          currentSession->OWDelaySum=0;
          currentSession->lastSequenceNum = msgHeaderPtr->sequenceNum;
        } else {

          //Msg received, update the session stats
          //Sessions lastArrivalTime is based on local high res timer
          thisInterArrivalTime= wallTime - currentSession->lastArrivalTimeD;
          thisJitter = thisInterArrivalTime - currentSession->thisInterArrivalTime;
          //if (thisJitter < 0)
          //  thisJitter=thisJitter*-1.0;

          currentSession->curJitter =  thisJitter;
          currentSession->thisInterArrivalTime = thisInterArrivalTime;

          currentSession->jitterSum+=thisJitter;
          currentSession->interArrivalTimeSum += thisInterArrivalTime;
          //Use this count when done to obtain the avg of all Sum session state vars
          currentSession->interArrivalTimeCount++;


          //If >0, likely means 1 or more packets dropped
          //tmpY = msgHeaderPtr->sequenceNum - currentSession->lastSequenceNum - 1;
          tmpY = actualSeqNumber - currentSession->lastSequenceNum - 1;

          //Example: 101 last to arrive. 105 just arrives.
          // So lastSequenceNum:101  clientSequenceNum is 105.
          // If no loss occcurs, clientSequenceNum would be 102.  105-101-1=3
          if (tmpY > 0)
          {
            currentSession->lossEventCount++;
            currentSession->messagesLost += (double)tmpY;

            printf("JJM:  actualSN:%f  tmpY:%d lastSN:%f messagesLost:%f \n",
               actualSeqNumber,tmpY, 
               currentSession->lastSequenceNum,
               currentSession->messagesLost);
          }

          if (tmpY <= 0){
           //Since we do not keep track of what arrives, we just count it as out of order
           currentSession->outOfOrderArrival++;
           //currentSession->duplicateArrival++;
          }

          currentSession->lastSequenceNum = actualSeqNumber;
          if (currentSession->largestSeqRecv < actualSeqNumber)
            currentSession->largestSeqRecv = actualSeqNumber;

          currentSession->delayChange = OWDsample - currentSession->curOWDelay;
          currentSession->delayChangeSum =
            currentSession->delayChangeSum +
            currentSession->delayChange;

          //OWDelay MUST be based on wall clock times
          currentSession->OWDelaySum+=OWDsample;
          if (OWDsample < 0)
            currentSession->countNOWDelaySamples++;
          else
            currentSession->countPOWDelaySamples++;

          if (wrapSeqNumber)
          {
            currentSession->sequenceNumberWrap++;
          }
#ifdef TRACEME
          printf("UDPEchoServer(%f): id:%d  messagesReceived:%f ,lastSN:%f, largest:%f\n", 
                    wallTime,
                    currentSession->sessionID, currentSession->messagesReceived, 
                    currentSession->lastSequenceNum,currentSession->largestSeqRecv);
#endif
        }
      }
    }
  }
}


void finalStats() 
{
  double  duration = 0.0;

  wallTime = getCurTimeD();
  endTime = wallTime;
  duration = endTime - startTime;

  printAllSessions(wallTime,stdout);

  if (sock != -1) {
     close(sock);
  }

  exit(EXIT_SUCCESS);
}


void CNTCCode() 
{

  //will not return....
  finalStats(); 
}



