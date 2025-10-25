// client_c_tcp.c
#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h> 
#define MAX 1024
#define SA struct sockaddr

//Portions of the socket setup code were adapted from:
//https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(port);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connection with the server failed");
        close(sockfd);
        exit(1);
    }

    char buff[MAX];
    bzero(buff, sizeof(buff));
    printf("Enter string: ");
    fgets(buff, sizeof(buff), stdin);

    write(sockfd, buff, strlen(buff));

    char recvbuff[MAX];
    int n;
    while ((n = read(sockfd, recvbuff, sizeof(recvbuff) - 1)) > 0) {
        recvbuff[n] = '\0';
        char *line = strtok(recvbuff, "\n");
        while (line != NULL) {
            printf("From server: %s\n", line);
            line = strtok(NULL, "\n");
        }
    }

    close(sockfd);
    return 0;
}
