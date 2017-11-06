#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

#define BUFFER_SIZE 10000

#define CONN_ALIVE 0
#define CONN_END 1
#define CONN_STOP_SEND 2

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

int handleLogic(int sockfd, int status_flag) {
    char recvline[BUFFER_SIZE];
    char* sendline = NULL;
    size_t length = 0;

    if(status_flag != CONN_STOP_SEND) {
        if(getline(&sendline, &length, stdin) == -1) {
            return CONN_STOP_SEND;
        }
        write(sockfd, sendline, strlen(sendline));
    }

    free(sendline);
    bzero(recvline, BUFFER_SIZE);
    if(read(sockfd, recvline, BUFFER_SIZE) == 0) {
        return CONN_END;
    }
    printf("%s", recvline);
    return CONN_ALIVE;
}

void connectionCallback(int sockfd, struct sockaddr_in servaddr) {
    int flag = CONN_ALIVE;
    do {
        flag = handleLogic(sockfd, flag);
    } while(flag != CONN_END);
    close(sockfd);
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
