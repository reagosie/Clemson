/************************************************************************
* File:  session.h
*
* Purpose:
*   This include file is for session includes/defines.
*
*  session *findActive(struct in_addr clientIP, uint16_t clientPort);
*  session *getActive(struct in_addr clientIP, uint16_t clientPort);
*  int removeActive(struct in_addr clientIP, uint16_t clientPort);
*  double updateSessionDuration(session *s);
*  int  printSessions(FiLE *fileID);
*  uint32_t freeAllSessions();
*
* Notes:
*
************************************************************************/
#ifndef	__session_h
#define	__session_h

#define MAX_SESSIONS 2048


typedef struct session {
  double  errorCount;  //tracks number of any type of error that might occur
  bool isActive;        //true if on the active list, false if on archived list
  uint32_t sessionID;   //uniquely id's this session 
  struct in_addr clientIP;
  uint16_t clientPort;
  //struct sockaddr_storage clientIP;
  uint16_t mode;
  struct timespec timeStarted;  
  struct timespec sessionEnd;  
  double timeStartedD;         //This should be a wall time 
  double firstArrivalTimeD;    //this should be based on a high precision clock

  double lastArrivalTimeD;
  double thisInterArrivalTime;
  double interArrivalTimeSum;
  double interArrivalTimeCount;

  //accurate based on timestamps
  double   timeFromFirstToLast;
  double   duration;
  double   bytesReceived;
  double   messagesReceived;
  double   bytesSent;
  double   messagesSent;
  double   messagesLost;
  double   lossEventCount;
  double lastSequenceNum;
  double  sequenceNumberWrap;
  double largestSeqRecv;
  double largestSeqSent;
  double  ArrivalsBeforeAck;
  double  outOfOrderArrival;
  double  duplicateArrival;
  double   curJitter;   //Jitter based on sequential packet arrivals 
  double   jitterSum;
  double countPOWDelaySamples; //Counts number of positive delays
  double countNOWDelaySamples; //Counts number of negative delays
  double   curOWDelay;
  double   OWDelaySum;
  double   delayChange;  //difference between this and the previous delay
  double   delayChangeSum;
  struct session *prev;
  struct session *next;
} session;


void initSessions();
int createSession(session  **sPtr);
int getNumberActiveSessions();
int getNumberSessions();

session *findActive(struct in_addr clientIP, uint16_t clientPort);
session *findSession(struct in_addr clientIP, unsigned short clientPort);
session *getActive(struct in_addr clientIP, uint16_t clientPort);

int removeActive(struct in_addr clientIP, uint16_t clientPort);
double updateSessionDuration(session *s);

int printSession(double curTime, FILE *fileFID, session *sPtr);
int printAllSessions(double curTime, FILE *fileFID);

int printActiveSessions(double curTime, FILE *fileFID); 

int printArchivedSessions(double curTime, FILE *fileFID); 
uint32_t freeAllSessions();

#endif

