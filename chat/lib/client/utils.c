#include "utils.h"

char * resolveHostname(char *hostname) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    char* ip;

    if ((he = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "Error: could not resolve hostname\n");
        exit(1);
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL;) {
        ip = malloc(INET_ADDRSTRLEN);
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return ip;
    }

    fprintf(stderr, "Error: could not resolve hostname\n");
    exit(1);
}

void sendGreetings(int sockfd, char *ip, int port, char *nickname) {
    Message greetingMessage = message(ip, port, createGreetingBody(nickname));
    sendMessage(sockfd, greetingMessage);
    printf("Connection stabilised\n");
}

