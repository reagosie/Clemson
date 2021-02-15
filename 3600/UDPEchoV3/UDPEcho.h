/*********************************************************
*
* Module Name: UDP Echo client/server header file
*
* File Name:    UDPEcho.h	
*
* Summary:
*  This file contains common stuff for the client and server
*
* Revisions:
*
*********************************************************/
#ifndef	__UDPEcho_h
#define	__UDPEcho_h
#include "common.h"


enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 512,
};


//defines max mesg size - needs to be a bit larger than possible max message
#define MAX_DATA_BUFFER 50028
//defines min mesg size - we'll assume can be as small as 4 bytes
#define MIN_DATA_BUFFER 4

//The following are the min/max amount of USER data supported in a single sendto/receive (i.e., UDP only!!)
//It does NOT include the overhead data needed / used by our message header (or any TCP/IP/Frame headers)

//Min must hold at lesat the messageHeader
#define MESSAGEMIN 12
#define MESSAGE_DEFAULT_SIZE 24
//Note: for UDP, this will cause frag. although if using localhost the mtu is usually >60Kbytes
#define MESSAGEMAX 50000

#define MAX_MSG_HDR 128


#define ECHOMAX 10000     /* Longest string to echo */
#define ERROR_LIMIT 5

typedef struct {
  uint32_t sequenceNum;
  uint32_t timeSentSeconds;
  uint32_t timeSentNanoSeconds;
} messageHeaderDefault;



#ifndef LINUX
#define INADDR_NONE 0xffffffff
#endif


#define MAX_TMP_BUFFER 1024


#endif

