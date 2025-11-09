# UDPPingerServer.py
import random
from socket import *

# Create a UDP socket
serverSocket = socket(AF_INET, SOCK_DGRAM)

# Assign IP address and port number to socket
serverSocket.bind(('', 12000))

while True:
    # Generate random number in range 0 to 10
    rand = random.randint(0, 10)
    # Receive the client packet along with the address it’s coming from
    message, address = serverSocket.recvfrom(1024)
    # Simulate packet loss
    if rand < 4:
        continue
    # Otherwise, echo the message back
    serverSocket.sendto(message, address)