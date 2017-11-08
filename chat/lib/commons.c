#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include "interfaces.h"

Message recvMessage(int sockfd) {
    MessageBody body;
    bzero(&body, sizeof body);

    struct sockaddr_storage storage;
    struct sockaddr_in *sin = (struct sockaddr_in *) &storage;

    socklen_t sin_len = sizeof(storage);
    bzero(&storage, sizeof(storage));

    recvfrom(sockfd, &body, sizeof body, 0, (struct sockaddr *) sin, &sin_len);

    Message message;
    message.body = body;
    bcopy(&sin->sin_addr, message.ip, INET_ADDRSTRLEN);
    message.port = ntohs(sin->sin_port);

    return message;
}

void sendMessage(int sockfd, Message message) {
    struct sockaddr_storage storage;
    struct sockaddr_in *sin = (struct sockaddr_in *) &storage;
    bzero(&storage, sizeof storage);

    sin->sin_family = AF_INET;
    bcopy(message.ip, &sin->sin_addr, INET_ADDRSTRLEN);
    sin->sin_port = message.port;

    sendto(sockfd, &message.body, sizeof message.body, 0, (struct sockaddr *) sin, sizeof(storage));
}

Message message(char *ip, uint16_t port) {
    Message m;
    bzero(&m, sizeof m);

    m.port = htons(port);
    if (inet_pton(AF_INET, ip, m.ip) <= 0) {
        perror("ERROR: inet_pton could to be concluded");
        exit(1);
    }
    inet_pton(AF_INET, ip, &m.ip);

    return m;
}

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR: Socket could not be created");
        exit(1);
    }

    return sockfd;
}

void bindSocket(int sockfd, uint16_t port) {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr) < 0) {
        perror("ERROR: Socket could not bind to port: " + port);
    }
}