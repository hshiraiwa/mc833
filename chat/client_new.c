#include <stdio.h>
#include <string.h>
#include "lib/commons.h"
#include "lib/interfaces.h"

int main() {
    printf("starting client\n");
    int sockfd = createSocket();
    printf("socket created\n");

    MessageBody body;
    strcpy((char *) body.message, "hello world");

    Message m = message("127.0.0.1", 3000, body);
    sendMessage(sockfd, m);
    printf("message send: %s\n", m.body.message);

    Message response = recvMessage(sockfd);
    printf("%s\n", response.body.message);

    return 0;
}