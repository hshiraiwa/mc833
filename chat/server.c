#include <stdio.h>
#include <string.h>
#include "lib/commons.h"
#include "lib/interfaces.h"

int main() {
    printf("starting server:\n");
    int sockfd = createSocket();
    bindSocket(sockfd, 3000);
    printf("socket bound to port 3000\n");

    for(;;) {
        Message m = recvMessage(sockfd);

        printf("%s %d - %s\n", m.ip, m.port, m.body.message);

        sendMessage(sockfd, m);
    }

    return 0;
}