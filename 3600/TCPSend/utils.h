/************************************************************************
* File:  utils.h
*
* Purpose:
*   This include file is for a set of utility functions 
*
* Notes:
*
************************************************************************/
#ifndef	__utils_h
#define	__utils_h

#include "common.h"
#include  <stdarg.h>		/* ANSI C header file */
#include  <syslog.h>


#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))
void setVersion(double versionLevel);
char *getVersion();


double timestamp();


double convertTimeval(struct timeval *t);
int udelay(int32_t ns);
int nanodelay(int64_t ns);
int myDelay(double delayTime);
int delay(int64_t ns);

void sig_chld(int signo);

void die(const char *msg);
void DieWithError(char *errorMessage); 
void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);

void swapbytes(void *_object, size_t size);
int packBufferWithInt(char *bufPtr, unsigned int intValue);
int is_bigendian();
#ifdef LINUX
uint64_t htonll(uint64_t InAddr) ;
#endif

ssize_t	 readline(int, void *, size_t);
ssize_t Readline(int fd, void *ptr, size_t maxlen);
void err_sys(const char *fmt, ...);

#define CHUNKSIZE 1000000
int  sendData(unsigned int transferSize, int writefd, int chunkSize);
int  RxData(unsigned int maxLength, int readfd, int chunkSize);


int getTime(struct timeval *callersTime);
double getCurTimeD();
double getCurTime(struct timespec *ts);
double getTimestampD(); 
double getTimeByType(int clockType);

int gettimeofday_benchmark();

long getMicroseconds(struct timeval *t);
double convertTimeval(struct timeval *t);

long getTimeSpan(struct timeval *start_time, struct timeval *end_time);
void setUnblockOption(int sock, char unblock);
void sockBlockingOn(int sock);
void sockBlockingOff(int sock);
#endif


