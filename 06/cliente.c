#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

#define BUFFER_SIZE 10000

void createConnection(char *ip, unsigned int port, void (*callback)(int, struct sockaddr_in)) {
    struct sockaddr_in servaddr;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket Error");
        exit(1);
    }

    //Setting servaddr parameters
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
        perror("inet_pton Error");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connect Error");
        exit(1);
    }

    callback(sockfd, servaddr);
}

int getPeerPort(struct sockaddr_in s) {
    return ntohs(s.sin_port);
}

char *getPeerIP(struct sockaddr_in s) {
    char *ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(s.sin_addr), ip, INET_ADDRSTRLEN);
    return ip;
}

void closeConnection(int sockfd) {
    close(sockfd);
    exit(0);
}

void handleLogic(int sockfd) {
    char recvline[BUFFER_SIZE];
    char sendline[BUFFER_SIZE];

    bzero(sendline, BUFFER_SIZE);
    bzero(recvline, BUFFER_SIZE);

    if(fgets(sendline, BUFFER_SIZE, stdin) == 0) {
            closeConnection(sockfd);
    }

    write(sockfd, sendline, strlen(sendline));
    read(sockfd, recvline, BUFFER_SIZE);
    printf("%s", recvline);
}

void connectionCallback(int sockfd, struct sockaddr_in servaddr) {
    while (1) handleLogic(sockfd);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        char error[BUFFER_SIZE];
        strcpy(error, "uso: ");
        strcat(error, argv[0]);
        strcat(error, " <IPaddress>");
        strcat(error, " <Port>");
        perror(error);
        return 1;
    }

    createConnection(argv[1], atoi(argv[2]), connectionCallback);
    return 0;
}
