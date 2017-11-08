#include <stdio.h>
#include <string.h>
#include "lib/commons.h"

int main() {
    printf("starting client\n");
    int sockfd = createSocket();
    printf("socket created\n");

    Message m = message("127.0.0.1", 3000);
    strcpy(m.body.message, "hello world");
    sendMessage(sockfd, m);
    printf("message send: %s\n", m.body.message);

    Message response = recvMessage(sockfd);
    printf("%s\n", response.body.message);

    return 0;
}