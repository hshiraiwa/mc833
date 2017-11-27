#include "utils.h"

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

void sendGreetings(int sockfd, char *ip, int port, char *nickname) {
    Message greetingMessage = message(ip, port, createGreetingBody(nickname));
    sendMessage(sockfd, greetingMessage);
    Message response;
    printf("Waiting for server ACK...\n");
    do {
        response = recvMessage(sockfd);
    } while(response.body.type != ACK && response.body.data.ack.code != GREETING);
    printf("Connection stabilised\n");
}

