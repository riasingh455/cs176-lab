CSIL Lab 3 – UDP Ping Client
Name: Ria Singh
Files included:
PingClient.c — UDP client implementation that sends 10 ping requests to the server and measures RTT.
Makefile — compiles the client with make.
How to run:
Start the server:
python3 UDPPingerServer.py
Compile the client:
make
Run the client:
./PingClient 127.0.0.1 12000
Description:
The client sends 10 UDP ping messages to the server, waits up to one second for each reply, and reports the RTT, packet loss, and min/avg/max times.