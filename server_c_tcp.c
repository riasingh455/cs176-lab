// server_c_tcp.c
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>   
#include <ctype.h>  

#define MAX 128
#define SA struct sockaddr

int is_all_digits(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

int sum_digits(const char *str) {
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            sum += str[i] - '0';
        }
    }
    return sum;
}

void int_to_str(int num, char *buff) {
    sprintf(buff, "%d", num);
}

void handle_client(int connfd) {
    char buff[MAX];
    bzero(buff, MAX);

    int n = read(connfd, buff, sizeof(buff));
    if (n <= 0) {
        close(connfd);
        return;
    }

    if (buff[strlen(buff) - 1] == '\n') {
        buff[strlen(buff) - 1] = '\0';
    }

    if (!is_all_digits(buff)) {
        char msg[] = "Sorry, cannot compute!";
        write(connfd, msg, strlen(msg));
        close(connfd);
        return;
    }

    int sum = sum_digits(buff);
    char sendbuff[MAX];

    while (sum >= 10) {
        int_to_str(sum, sendbuff);
        write(connfd, sendbuff, strlen(sendbuff));
        write(connfd, "\n", 1);
        sum = sum_digits(sendbuff);
    }

    int_to_str(sum, sendbuff);
    write(connfd, sendbuff, strlen(sendbuff));
    write(connfd, "\n", 1);

    close(connfd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    servaddr.sin_port = htons(port);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    if ((listen(sockfd, 5)) != 0) {
        perror("listen failed");
        close(sockfd);
        exit(1);
    }

    len = sizeof(cli);
    while (1) {
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            perror("accept failed");
            continue;
        }

        handle_client(connfd);
    }

    close(sockfd);
    return 0;
}
