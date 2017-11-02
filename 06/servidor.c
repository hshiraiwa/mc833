#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 10
#define BUFFER_SIZE 200

int getPeerPort(struct sockaddr_in s) {
    return ntohs(s.sin_port);
}

char *getPeerIP(struct sockaddr_in s) {
    char *ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(s.sin_addr), ip, INET_ADDRSTRLEN);
    return ip;
}

void logString(struct sockaddr_in s, const char *data) {
    time_t ticks = time(NULL);
    char *ip = getPeerIP(s);
    int port = getPeerPort(s);
    FILE *file = fopen("servidor.log", "a");
    fprintf(file, "%.24s - %s:%d - %s\n", ctime(&ticks), ip, port, data);
    fclose(file);
    free(ip);
}

void handleConnection(int listenfd, void (*callback)(int, struct sockaddr_in)) {
    struct sockaddr_in storage;
    socklen_t storage_len = sizeof(storage);

    while (1) {
        int connfd = accept(listenfd, (struct sockaddr *) &storage, &storage_len);
        if (connfd < 0) {
            perror("Connection Error");
            exit(1);
        }
        if (fork() == 0) {
            close(listenfd);
            logString(storage, "Connected");
            callback(connfd, storage);
            close(connfd);
            logString(storage, "Disconnected");
            exit(0);
        }

        close(connfd);
    }

}

void createServer(unsigned int port, void (*callback)(int, struct sockaddr_in)) {
    struct sockaddr_in servaddr;
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket Error");
        exit(1);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        perror("Bind Error");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) == -1) {
        perror("Listen Error");
        exit(1);
    }

    handleConnection(listenfd, callback);
    return;
}

void connectionCallback(int connfd, struct sockaddr_in connaddr) {
    char buf[BUFFER_SIZE];

    while (1) {
        int read = recv(connfd, buf, BUFFER_SIZE, 0);
        if(read <= 0)
            return;

        send(connfd, buf, read, 0);
    }
}

int main(int argc, char **argv) {
    createServer(atoi(argv[1]), connectionCallback);

    return (0);
}
