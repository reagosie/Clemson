/************************************************************************
* File:  AddressHelper.h
*
* Purpose:
*   This is the include file for the AddressHelper module
*
* Notes:
*   Code should always exit using Unix convention:  exit(EXIT_SUCCESS) or exit(EXIT_FAILURE)
*
************************************************************************/
#ifndef	__AddressHelper_h
#define	__AddressHelper_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


// Print socket address
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
int findAF_INET6SocketAddress(struct addrinfo *servAddr, struct sockaddr **callersaddresss);
int findAF_INETSocketAddress(struct addrinfo *servAddr, struct sockaddr **callersaddresss); 

// Test socket address equality
bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);


int NumberOfAddresses(struct addrinfo *addrList);

#ifndef LINUX
#define INADDR_NONE 0xffffffff
#endif


#endif


