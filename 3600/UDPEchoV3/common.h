/************************************************************************
* File:  common.h
*
* Purpose:
*   This include file is for common includes/defines.
*
* Notes:
*
************************************************************************/
#ifndef	__common_h
#define	__common_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  /*brings in C99 types of uint32_t uint64_t  ...*/
#include <limits.h>  /*brings in limits such as LONG_MIN LLONG_MAX ... */
#include <math.h>    /* floor, ... */

#include <string.h>     
#include <errno.h>

#include <stdbool.h>

#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <netinet/in.h> /* for in_addr */
#include <arpa/inet.h> /* for inet_addr ... */

#include <unistd.h>     /* for close() */
#include <fcntl.h>
#include <netdb.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>



//Definition, FALSE is 0,  TRUE is anything other
#define TRUE 1
#define FALSE 0

#define VALID 1
#define NOTVALID 0

//Defines max size temp buffer that any object might create
#define MAX_TMP_BUFFER 1024
#define MAX_LINE_SIZE 1024

/*
  Consistent with C++, use EXIT_SUCCESS, SUCCESS is a 0, otherwise  EXIT_FAILURE  is not 0

  If the method returns a valid rc, EXIT_FAILURE is <0
   Should use bool when the method has a context appropriate for returning T/F.   
*/
#define SUCCESS 0
#define NOERROR 0

#define ERROR   -1
#define FAILURE -1
#define FAILED -1


#define CHAR_ERROR 0xFF
#define DOUBLE_ERROR  -1.0


//We assume a NULL can be interpretted as an error

void die(const char *msg);
void DieWithError(char *errorMessage); /* External error handling function */
// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);

#endif


