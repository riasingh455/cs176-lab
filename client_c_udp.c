// client_c_udp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in servaddr;
    char sendbuff[MAXLINE], recvbuff[MAXLINE];
    socklen_t addrlen = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(port);


    printf("Enter string: ");

    size_t total = 0;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF && total < sizeof(sendbuff) - 1) {
        sendbuff[total++] = ch;
    }
    sendbuff[total] = '\0';


    if (sendto(sockfd, sendbuff, strlen(sendbuff), 0,
               (struct sockaddr*)&servaddr, addrlen) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(1);
    }

    while (1) {
        bzero(recvbuff, sizeof(recvbuff));
        int n = recvfrom(sockfd, recvbuff, sizeof(recvbuff) - 1, 0,
                         (struct sockaddr*)&servaddr, &addrlen);
        if (n <= 0) {
            break;  
        }
        recvbuff[n] = '\0';
        printf("From server: %s\n", recvbuff);

        if (strlen(recvbuff) == 1 || strncmp(recvbuff, "Sorry", 5) == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
