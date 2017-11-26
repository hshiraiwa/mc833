#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "lib/commons.h"

void resolveHostname(char *hostname, char **ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ((he = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "Error: could not resolve hostname\n");
        exit(1);
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL;) {
        *ip = malloc(INET_ADDRSTRLEN);
        strcpy(*ip, inet_ntoa(*addr_list[i]));
        return;
    }

    fprintf(stderr, "Error: could not resolve hostname\n");
    exit(1);
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("usage: main <hostname> <port> <nickname>\n");
        return 0;
    }

    char *ip;
    resolveHostname(argv[1], &ip);
    uint16_t port = (uint16_t) atoi(argv[2]);

    int sockfd = createSocket();

    Message greetingMessage = message(ip, port, createGreetingBody(argv[3]));
    sendMessage(sockfd, greetingMessage);
    recvMessage(sockfd);
    sleep(3);
    MessageBody body = createMessageBody("Hello World");
    for (int i = 0; i < 4; i++) {
        Message m = message(ip, port, body);
        sendMessage(sockfd, m);
        printf("message send: %s %d\n", m.ip, m.port);

        Message response = recvMessage(sockfd);
        printf("%s\n", response.body.data.message);
    }

    return 0;
}