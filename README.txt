CS176A — Homework 2: Socket Programming  
Author: Ria Singh

------------------------------------------------------------
FILES
- server_c_tcp.c     : TCP server
- client_c_tcp.c     : TCP client
- server_c_udp.c     : UDP server
- client_c_udp.c     : UDP client
- Makefile           : Builds all programs
- README.txt         : Instructions

------------------------------------------------------------
DESCRIPTION
- TCP server/client: Uses sockets to send a string from client to server.  
  Server checks if the string is numeric. If not, replies with:
      "Sorry, cannot compute!"
  If numeric, server sums the digits and keeps sending results until one digit remains.

- UDP server/client: Does the same as TCP, but with connectionless communication.

------------------------------------------------------------
COMPILE
make

------------------------------------------------------------
RUN
# TCP
./server_c_tcp 32000
./client_c_tcp 127.0.0.1 32000

# UDP
./server_c_udp 32000
./client_c_udp 127.0.0.1 32000

Example:
Input: 12345  
Output:
From server: 15  
From server: 6

------------------------------------------------------------
REFERENCES
- GeeksforGeeks TCP/UDP C socket examples  
- CS176A Lecture Notes  
https://www.geeksforgeeks.org/computer-networks/udp-client-server-using-connect-c-implementation/
https://www.geeksforgeeks.org/c/tcp-server-client-implementation-in-c/
then modified to meet assignment requirements.
