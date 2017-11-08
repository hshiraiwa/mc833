#include <stdio.h>
#include <string.h>
#include "lib/commons.h"
#include "lib/interfaces.h"

int main() {
    printf("starting server:\n");
    int sockfd = createSocket();
    bindSocket(sockfd, 3000);
    printf("socket bound to port 3000\n");

    while (1) {
        Message m = recvMessage(sockfd);
        printf("%s\n", m.body.message);
        if(strcmp(m.body.message, "exit\n") == 0)
            break;
        sendMessage(sockfd, m);
    }

    return 0;
}