
********************************************************
*  
*  readme.txt : Exercise 2 solution 
*
*  You were to extend simpleUDPEcho (described in donaho ch 4)
*   to accept arbitrarily large input strings. 
*
*  There are many  ways to do this....the implementation
*  provided is meant to serve as a stepping stone to HW1 Question 3  (UDPEcho)
*
*
*  last update: 2/2/2020
*
********************************************************


The server is relatively unchanged - just needed to allocate a large buffer to support
the largest allowed client message size

  server <Server Port/Service>


The client accepts new program parameters
Iteration Delay :   the delay between iterations passed in units of microseconds...currently 
  not supported by the client.
message Size:  The amount of data to send to the server
NumberOfIterations:  The number of times the client should loop. A value of 0 implies loop forever.


simpleUDPEcho(v1.0): <Server IP> <Server Port> <Iteration Delay (usecs)> <Message Size (bytes)>] <# of iterations> 



