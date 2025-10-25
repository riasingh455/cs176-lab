// server_c_udp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 128

int is_all_digits(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') continue;
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int sum_digits(const char *str) {
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) sum += str[i] - '0';
    }
    return sum;
}

void int_to_str(int num, char *buff) {
    sprintf(buff, "%d", num);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    printf("UDP server listening on port %d\n", port);

    len = sizeof(cliaddr);
    while (1) {
        bzero(buffer, MAXLINE);

        int n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                         (struct sockaddr*)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';

        if (!is_all_digits(buffer)) {
            char msg[] = "Sorry, cannot compute!";
            sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr*)&cliaddr, len);
            continue;
        }

        int sum = sum_digits(buffer);
        char sendbuff[MAXLINE];
        while (sum >= 10) {
            int_to_str(sum, sendbuff);
            sendto(sockfd, sendbuff, strlen(sendbuff), 0,
                   (struct sockaddr*)&cliaddr, len);
            sum = sum_digits(sendbuff);
        }

        int_to_str(sum, sendbuff);
        sendto(sockfd, sendbuff, strlen(sendbuff), 0,
               (struct sockaddr*)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
