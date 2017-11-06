#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

#define BUFFER_SIZE 10000
#define TIMEOUT 3600

#define KEEP_CONN 0
#define END_CONN 1
#define STOP_SENDING 2

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

char* readPoll(int sockfd){
    struct pollfd pfd[1];
    char* buf = malloc(sizeof(char) * BUFFER_SIZE);
    bzero(buf, BUFFER_SIZE);
    pfd->fd = sockfd;
    pfd->events = POLLIN;
    int rv = poll(pfd, 1, TIMEOUT);

     if( rv <= 0 || (pfd[0].revents & (POLLERR | POLLHUP)) ) {
        return NULL;
    }

    if(pfd[0].revents & POLLIN) {
        if(read(sockfd, buf, BUFFER_SIZE) == 0){
            free(buf);
            return NULL;
        }
        return buf;
    }
    return NULL;
}

void writePoll(int sockfd, char* input){
    struct pollfd pfd[1];
    pfd->fd = sockfd;
    pfd->events = POLLOUT;
    int rv = poll(pfd, 1, TIMEOUT);

    if(pfd[0].revents & POLLOUT) {
        write(sockfd, input, strlen(input));
        return;
    }
}

int handleLogic(int sockfd, int status_flag) {
    char* recvline = NULL;
    char* sendline = NULL;
    size_t length = 0;

    if(status_flag != STOP_SENDING) {
        if(getline(&sendline, &length, stdin) == -1) {
            return STOP_SENDING;
        }
        writePoll(sockfd, sendline);
    }

    recvline = readPoll(sockfd);
    if(recvline == NULL) {
        return END_CONN;
    }
    printf("%s", recvline);
    free(recvline);
    return KEEP_CONN;
}

void connectionCallback(int sockfd, struct sockaddr_in servaddr) {
    int flag = KEEP_CONN;
    do {
        flag = handleLogic(sockfd, flag);
    } while(flag != END_CONN);
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
