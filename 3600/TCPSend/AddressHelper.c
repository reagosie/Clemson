/*********************************************************
*
* Module Name: AddressHelper 
*
* File Name:  AddressHelper.c
*
* Summary:  This holds helpful routines related to addresses 
* Revisions:
*
* Last update: 4/8/2020
*
*  -Merged AddressUility with this file
*
*********************************************************/
#include "AddressHelper.h"
#include "utils.h"


/***********************************************************
* Function: int NumberOfAddress(const struct sockaddr *address) 
*
* Explanation:  Returns the number of addresses in the list 
*
* inputs:   
*   const struct addrinfo  *addrList) 
*
* outputs: returns a EXIT_FAILURE (-1) or valid number of addresses
*
**************************************************/
int NumberOfAddresses(struct addrinfo *addrList)
{
int rc = EXIT_FAILURE;
int counter = 0;
struct addrinfo *addr=NULL;

  if (addrList == NULL)
    rc = EXIT_FAILURE;
  else {
    for (addr = addrList; addr != NULL; addr = addr->ai_next) {
      counter++;
//      PrintSocketAddress(addr->ai_addr, stdout);
//      fputc('\n', stdout);
    }
    rc = counter;
  }
  return rc;
} 


/***********************************************************
* Function: void PrintSocketAddress(const struct sockaddr *address, FILE *stream) 
*
* Explanation: prints all addresses in the sockaddr
*
* inputs:   
*    const struct sockaddr *address) 
*    FILE *stream : output stream descriptor
*
* outputs: 
*
**************************************************/
void PrintSocketAddress(const struct sockaddr *address, FILE *stream) 
{
  // Test for address and stream
  if (address == NULL || stream == NULL)
    return;

  void *numericAddress; // Pointer to binary address

  // Buffer to contain result (IPv6 sufficient to hold IPv4)
  char addrBuffer[INET6_ADDRSTRLEN];
  in_port_t port; // Port to print
  // Set pointer to address based on address family
  switch (address->sa_family) {
  case AF_INET:
    numericAddress = &((struct sockaddr_in *) address)->sin_addr;
    port = ntohs(((struct sockaddr_in *) address)->sin_port);
    break;
  case AF_INET6:
    numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
    port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
    break;
  case AF_PACKET:
    fputs("address Family AF_PACKET, ignore.... ", stream);   //
    return;
  default:
    fputs("[unknown type]", stream);    // Unhandled type
    return;
  }
  // Convert binary to printable address (presentation format)
  if (inet_ntop(address->sa_family, numericAddress, addrBuffer,
      sizeof(addrBuffer)) == NULL)
    fputs("[invalid address]", stream); // Unable to convert
  else {
    fprintf(stream, "%s", addrBuffer);
    if (port != 0)                // Zero not valid in any socket addr
      fprintf(stream, "-%u", port);
  }
}

bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2) {
  if (addr1 == NULL || addr2 == NULL)
    return addr1 == addr2;
  else if (addr1->sa_family != addr2->sa_family)
    return false;
  else if (addr1->sa_family == AF_INET) {
    struct sockaddr_in *ipv4Addr1 = (struct sockaddr_in *) addr1;
    struct sockaddr_in *ipv4Addr2 = (struct sockaddr_in *) addr2;
    return ipv4Addr1->sin_addr.s_addr == ipv4Addr2->sin_addr.s_addr
        && ipv4Addr1->sin_port == ipv4Addr2->sin_port;
  } else if (addr1->sa_family == AF_INET6) {
    struct sockaddr_in6 *ipv6Addr1 = (struct sockaddr_in6 *) addr1;
    struct sockaddr_in6 *ipv6Addr2 = (struct sockaddr_in6 *) addr2;
    return memcmp(&ipv6Addr1->sin6_addr, &ipv6Addr2->sin6_addr,
        sizeof(struct in6_addr)) == 0 && ipv6Addr1->sin6_port
        == ipv6Addr2->sin6_port;
  } else
    return false;
}


/*************************************************************
*
* Function: int findAF_INET6SocketAddress(struct addrinfo *,
*                   struct sockaddr **callersaddress) 
* 
* Summary:  attempts to find an IP V6 addr in the list.  Returns ERROR if can not find one.
*            else fills in the caller's addr ptr with the address and returns NOERROR.
*
* Inputs:
*   struct addrinfo *  : reference to callers addrinfo  
*   struct sockaddr **callersaddress : reference to the callers address ptr that is to be filled in
*           
*
* outputs:  
*     updates callersaddress if an IPV6 addr is found in the list. 
*     returns the number of IPV6 address that are found.  0 means none found.
*     If > 1, the callersaddress is filled in with the first
*
* notes: 
*    
*
***************************************************************/
int findAF_INET6SocketAddress(struct addrinfo *servAddr, struct sockaddr **callersaddress) 
{
  struct addrinfo *addr = NULL;
  uint32_t count =0;


  for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
    if (addr->ai_addr->sa_family == AF_INET6) {
      if (count == 0)
        *callersaddress = addr->ai_addr;
      count++;
    }
  }

#ifdef TRACE
  printf("findAF_INET6SocketAddress: Found %d V6 addresses \n", count);
#endif
  return count;
}

/*************************************************************
*
* Function: int findAF_INETSocketAddress(const struct sockaddr *address, struct sockaddr **callersaddress) 
* 
* Summary:  attempts to find an IP V4 addr in the list.  Returns ERROR if can not find one.
*            else fills in the caller's addr ptr with the first V4 addr in the list
*           and returns NOERROR.
*
* Inputs:
*   const struct sockaddr *address : reference to address of interest 
*   struct sockaddr **callersaddress : reference to the callers address ptr that is to be filled in
*           
*
* outputs:  
*     updates callersaddress if an IPV4 addr is found. Returns the number of
*        IPV4 addresses found. A 0 means none found. 
*
* notes: 
*    
*
***************************************************************/
int findAF_INETSocketAddress(struct addrinfo *servAddr, struct sockaddr **callersaddress) 
{
  struct addrinfo *addr = NULL;
  uint32_t count =0;

  for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
    if (addr->ai_addr->sa_family == AF_INET) {
      if (count == 0)
        *callersaddress = addr->ai_addr;
      count++;
    }
  }

#ifdef TRACE
  printf("findAF_INETSocketAddress: Found %d V4 addresses \n", count);
#endif

  return count;
}


